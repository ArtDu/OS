#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>

typedef enum {
    INIT, LIST_OF_CHATS, CLIENT_OFF,
    ADD_CHAT, JOIN_TO_CHAT, UNSUB,
    SEND_MESSAGE, SEND_TO_SUBS, SHOW_HISTORY
} RequestToken;

typedef struct {    
  RequestToken action;
  char clientName[256];
  pid_t client_proc_id;
  char chatName[256];
  char text[256];
  long long status;
} Message ;

typedef struct {
    std::vector<std::string> usersInChat;
    std::string name;
} ChatElem ;


#endif