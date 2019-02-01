#include "data.h"
#include <zmq.h>
#include <map>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <signal.h>
#include <fstream>


char ans_publ[256];
int END = 0;
int NODE = 1;


int main(int argc, char *argv[]) {

    int countOfClients = 0;
    std::vector<std::pair<std::string, std::string> > history;
    std::vector<std::pair<std::vector<std::string>, std::string> > debtors; // users, message
    std::map<std::string, pid_t> tree; // map with clients <name, pid>
    // if pid == 0, client is offline
    std::vector<ChatElem> chats;
    Message *message;
    zmq_msg_t reply;
    zmq_msg_t request;
    char ans[256];


    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    ans[0] = '\0';
    strcat(ans, "tcp://*:");
    char *port;
    if (argc == 2) {
        port = argv[1];

    } else {
        port = "4040";
    }
    strcat(ans, port);

    std::ifstream in("clients.txt", std::ios::in);
    if (in.is_open()) {
        while (1) {

            int ctmp;
            in >> ctmp;

            if (ctmp == END) {
                break;
            }


            std::string str;
            size_t size;
            in >> size;
            in >> str;

            pid_t status;

            in >> status;
            /*std::cout << ctmp << "\n";
            std::cout << str << "\n";
            std::cout << status << "\n\n";*/
            tree[str] = status;

        }
    }

    std::ifstream in_history("history.txt", std::ios::in);
    if (in_history.is_open()) {
        while (1) {

            int ctmp;
            in_history >> ctmp;

            if (ctmp == END) {
                break;
            }


            std::string name, mes;
            in_history >> name;
            std::getline(in_history, mes);

            std::pair<std::string, std::string> chat_message = std::make_pair(name, mes);
            history.push_back(chat_message);


        }
    }

    std::ifstream in_chats("chats.txt", std::ios::in);
    if (in_chats.is_open()) {
        while (1) {

            int ctmp;
            in_chats >> ctmp;
            //std::cout << ctmp << "\n";

            if (ctmp == END) {
                break;
            }

            ChatElem elem;
            in_chats >> elem.name;
            //std::cout << elem.name << "\n";

            while (1) {
                std::string name;
                pid_t status;
                in_chats >> name;
                if (name == "-1") {
                    break;
                }
                in_chats >> status;
                std::pair<std::string, pid_t> user = std::make_pair(name, status);
                elem.usersInChat.push_back(user);
                //std::cout << name << " " << status << "\n\n";
            }
            chats.push_back(elem);

        }
    }


    //std::vector<std::pair<std::vector<std::string>, std::string> > debtors; // users, message
    std::ifstream in_debtors("debtors.txt", std::ios::in);
    if (in_debtors.is_open()) {
        while (1) {

            int ctmp;
            in_debtors >> ctmp;
            //std::cout << ctmp << "\n";

            if (ctmp == END) {
                break;
            }

            std::string mes;
            std::getline(in_debtors, mes);
            //std::cout << mes << "\n";
            std::vector<std::string> users;
            while (1) {
                std::string user;
                in_debtors >> user;
                if (user == "-1") {
                    break;
                }
                //std::cout << user << " ";
                users.push_back(user);
            }
            //std::cout << std::endl;
            std::pair<std::vector<std::string>, std::string> one_mes = make_pair(users, mes);
            debtors.push_back(one_mes);
        }
    }

    int rc = zmq_bind(responder, ans);
    if (rc != 0) {
        perror("zmq_bind");
        zmq_close(responder);
        zmq_ctx_destroy(context);
        exit(1);
    }


    printf("Server initialized\n");
    for (;;) {
        zmq_msg_init(&request);
        zmq_msg_recv(&request, responder, 0);
        message = (Message *) zmq_msg_data(&request);
        printf("Recieved message from %s action: %d \n", message->clientName, message->action);
        ans[0] = '\0';
        std::string clientName(message->clientName);

        std::map<std::string, pid_t>::iterator it;
        it = tree.find(clientName);
        if (it == tree.end()) {
            //register new id
            printf("Client %s added successfully\n", message->clientName);
            tree[clientName] = message->client_proc_id;
        }
        else if (it->second != message->client_proc_id) {
            it->second = message->client_proc_id;
            for(auto &i : debtors) {
                //std::vector<std::pair<std::vector<std::string>, std::string> > debtors; // users, message
                for (auto &j : i.first) {
                    if(clientName == j) {

                        strcat(ans, i.second.c_str());
                        strcat(ans, "\n");

                        std::vector<std::string>::iterator it_vec;
                        it_vec = find(i.first.begin(), i.first.end(), j);
                        i.first.erase(it_vec);
                        break;
                    }
                }
            }
        }

        if (INIT == message->action) {
            it = tree.find(message->clientName);
            if (it != tree.end()) {
                strcat(ans, "OK");
                tree[message->clientName] = message->client_proc_id;
                countOfClients++;
            } else
                sprintf(ans, "ERROR");
        } else if (CLIENT_OFF == message->action) {
            countOfClients--;
            tree[message->clientName] = 0;
            if (countOfClients <= 0) {
                printf("all clients are offline, server was down\n");
                break;
            }
        } else if (LIST_OF_CHATS == message->action) {
            if (chats.empty()) {
                sprintf(ans, "No chats");
            } else {
                std::string chat = "Chats name(users in chat):\n";
                for (auto i : chats) {
                    chat += i.name + "(";
                    for (auto j : i.usersInChat) {
                        chat += j.first + ", ";
                    }
                    chat += ")\n";

                }
                sprintf(ans, chat.c_str());
            }
        } else if (ADD_CHAT == message->action) {

            ChatElem tmp;
            tmp.name = message->text;
            chats.push_back(tmp);

            sprintf(ans, "Chat added");
        } else if (JOIN_TO_CHAT == message->action) {
            bool success = 0;
            for (int i = 0; i < chats.size(); ++i) {
                if (strcmp(chats[i].name.c_str(), message->text) == 0) {
                    std::pair<std::string, pid_t> tmp(clientName, message->client_proc_id);
                    chats[i].usersInChat.push_back(tmp);
                    success = 1;
                }
            }

            if (success) {

                sprintf(ans, "Client joined to %s", message->text);
            } else {
                sprintf(ans, "error");
            }
        } else if (UNSUB == message->action) {
            bool success = false;
            for (int i = 0; i < chats.size(); ++i) {
                if (strcmp(chats[i].name.c_str(), message->text) == 0) {
                    std::vector<std::pair<std::string, pid_t>>::iterator it_vec;
                    std::pair<std::string, pid_t> tmp(clientName, message->client_proc_id);
                    it_vec = find(chats[i].usersInChat.begin(), chats[i].usersInChat.end(), tmp);
                    if (it_vec != chats[i].usersInChat.end()) {
                        success = true;
                        chats[i].usersInChat.erase(it_vec);
                    } else {
                        success = false;
                    }

                }
            }

            if (success) {
                sprintf(ans, "User %s unsub from chat %s", clientName.c_str(), message->text);
            } else {
                sprintf(ans, "error");
            }
        } else if (SEND_MESSAGE == message->action) {
            bool success_chat = false;
            bool success_user_in_chat = false;

            ans_publ[0] = '\0';


            strcat(ans_publ, "(");
            strcat(ans_publ, message->chatName);
            strcat(ans_publ, ") ");
            strcat(ans_publ, message->clientName);
            strcat(ans_publ, ": ");
            strcat(ans_publ, message->text);

            std::string chatName(message->chatName);
            std::string mess(ans_publ);
            std::pair<std::string, std::string> tmp(chatName, mess);
            history.push_back(tmp);


            for (auto i : chats) {
                if (strcmp(i.name.c_str(), message->chatName) == 0) {
                    success_chat = true;

                    for (auto j : i.usersInChat) {
                        if (j.first == clientName) {
                            success_user_in_chat = true;
                            break;
                        }
                    }
                    if (success_user_in_chat) {
                        std::vector<std::string> users;
                        for (auto j : i.usersInChat) {

                            if (tree[j.first] == 0) {//std::vector<std::pair<std::vector<std::string>, std::string> > debtors; // users, message
                                users.push_back(j.first);
                            }

                            else if (tree[j.first] != message->client_proc_id) {

                                kill(tree[j.first], SIGUSR1);
                            }
                        }
                        if (!users.empty()) {
                            std::string chat_message(ans_publ);
                            std::pair<std::vector<std::string>, std::string> debtor = make_pair(users, chat_message);
                            debtors.push_back(debtor);
                        }
                    }
                }
            }


            if (success_chat) {
                if (success_user_in_chat) {
                    sprintf(ans, "Ok");
                } else {
                    sprintf(ans, "User isn't in chat");
                }
            } else {
                sprintf(ans, "Chat isn't exist");
            }

        } else if (SHOW_HISTORY == message->action) {
            ans[0] = '\0';
            for (auto i : history) {
                if (strcmp(i.first.c_str(), message->chatName) == 0) {

                    strcat(ans, i.second.c_str());
                    strcat(ans, "\n");
                }
            }

        } else if (SEND_TO_SUBS == message->action) {
            bool success = true;

            strcat(ans, ans_publ);


        } else {
            sprintf(ans, "ERROR: Wrong request");
        }

        printf("Send answer to client: [%s]\n", ans);
        zmq_msg_close(&request);
        zmq_msg_init_size(&reply, strlen(ans) + 1);
        memcpy(zmq_msg_data(&reply), ans, strlen(ans) + 1);
        zmq_msg_send(&reply, responder, 0);
        zmq_msg_close(&reply);
    }


    //tree
    std::ofstream out("clients.txt", std::ios::out);
    for (auto &kv : tree) {
        out << NODE << " ";
        size_t size = kv.first.size();
        out << size << " ";
        out.write(&kv.first[0], size);
        out << " ";
        out << kv.second;
        out << "\n";
    }
    out << END;
    out.flush();
    out.close();

    //std::vector<ChatElem> chats;
    std::ofstream out_chats("chats.txt", std::ios::out);
    for (auto &kv : chats) {
        out_chats << NODE << " ";
        out_chats << kv.name << " ";
        for (auto &user : kv.usersInChat) {
            out_chats << user.first << " " << user.second << " ";
        }
        out_chats << "-1";
        out_chats << "\n";
    }
    out_chats << END;
    out_chats.flush();
    out_chats.close();


    //std::vector<std::pair<std::string, std::string> > history;
    std::ofstream out_history("history.txt", std::ios::out);
    for (auto &kv : history) {
        out_history << NODE << " ";
        out_history << kv.first << " ";
        out_history << kv.second << "\n";
        out_history << "\n";
    }
    out_history << END;
    out_history.flush();
    out_history.close();

    //std::vector<std::pair<std::vector<std::string>, std::string> > debtors; // users, message
    std::ofstream out_debtors("debtors.txt", std::ios::out);
    for (auto &kv : debtors) {
        out_debtors << NODE << " ";
        out_debtors << kv.second << "\n";
        for (auto &user : kv.first) {
            out_debtors << user << " ";
        }
        out_debtors << "-1";
        out_debtors << "\n";
    }
    out_debtors << END;
    out_debtors.flush();
    out_debtors.close();

    zmq_close(responder);
    zmq_ctx_destroy(context);
    return 0;
}
