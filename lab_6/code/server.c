#include "vector.h"
#include <zmq.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int END = 0;

int main(int argc, char *argv[]) {
  int countOfClients = 0;
  Vector* vector = VectorCreate(10);
  Message* message;
  zmq_msg_t reply;
  zmq_msg_t request;
  char ans[256];
  void* context = zmq_ctx_new();
  void* responder = zmq_socket(context, ZMQ_REP);
  ans[0]='\0';
  strcat(ans,"tcp://*:");
  char* port;
  if(argc==2) {
  	port = argv[1];
  	
  }
  else {
  	port = "4040";
  }
  strcat(ans,port);

  FILE* fd;
  fd = fopen(port, "rb");
  if (fd != NULL)
  {
  	while (1) {
  		int id;
  		long long money;
			fread( &id, sizeof( int ), 1, fd );
			if(id == END) {
				break;
			}
			fread( &money, sizeof( long long ), 1, fd );
			VectorAdd(vector, id);
			CashToDebit(vector, money, id);
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
  for(;;){
		zmq_msg_init(&request);
		zmq_msg_recv(&request, responder, 0);
		message = (Message*) zmq_msg_data(&request);
		printf("Recieved message from %d action: %d \n", message->clientId, message->action);

		if (VectorSearch(vector, message->clientId)<0) {
			//register new id
			printf("Client %d added successfully\n",message->clientId);
			VectorAdd(vector, message->clientId);

		}

		if (INIT == message->action) {
			if(VectorSearch(vector, message->clientId)>=0) {
		        sprintf(ans, "OK");
		        countOfClients++;
		    }
			else
				sprintf(ans, "ERROR");

		} else if (ADD_DEBIT == message->action) {
			if(CashToDebit(vector, message->amount, message->clientId)==0)
				sprintf(ans, "OK");
			else
				sprintf(ans, "ERROR");

		} else if (WTHDRAW_DEBIT == message->action) {
			int ret = Withdraw(vector, message->amount, message->clientId);
			if(ret==0)
				sprintf(ans, "OK");
			else if(ret==1)
				sprintf(ans, "OK: lack of funds withdrew from credit account");
			else if(ret==2)
				sprintf(ans, "Not enough money in the account");
			else
				sprintf(ans, "ERROR");

		} else if (STAT == message->action) {
			if(getStat(vector, message->clientId, ans)!=0)
				sprintf(ans, "ERROR");

		} else if (CLIENT_OFF == message->action) {
			countOfClients--;
			//printf("%d\n", countOfClients);
			if(countOfClients<=0) {
				printf("all clients are offline, server was down\n");
				break;
			}
		} else if (SEND == message->action) {
			if(VectorSearch(vector, message->receiverId)<0)
				sprintf(ans, "No such receiver exist");
			else {
				int ret = Withdraw(vector, message->amount, message->clientId);
				if(ret==0)
					sprintf(ans, "OK");
				else if(ret==1)
					sprintf(ans, "OK: lack of funds withdrew from credit account");
				else if(ret==2)
					sprintf(ans, "Not enough money in the account");
				else
					sprintf(ans, "ERROR");

				if(ret==1||ret==0)
					if(CashToDebit(vector, message->amount, message->receiverId)!=0){
						sprintf(ans, "ERROR");
						//if error occured  transaction failed
						//return money back to sender
						if(CashToDebit(vector, message->amount, message->clientId)!=0){
							printf("FATAL ERROR: cannot revoke transaction\n ");
							printf("\tcan not give back %lld moneys to %d\n ", message->amount,  message->clientId);
							break;
						}

					}
			}

		}  else {
			sprintf(ans, "ERROR: Wrong request");
		}

		printf("Send answer to client: [%s]\n", ans);
		zmq_msg_close(&request);
		zmq_msg_init_size(&reply, strlen(ans)+1);
		memcpy(zmq_msg_data(&reply), ans, strlen(ans)+1);
		zmq_msg_send(&reply, responder, 0);
		zmq_msg_close(&reply);
  }

  fd = fopen(port, "wb");
	for ( int i = 0; i < vector->used; ++i ) {
		fwrite( &vector->vector[i].id, sizeof( vector->vector[i].id ), 1, fd );
		fwrite( &vector->vector[i].debit, sizeof( vector->vector[i].debit ), 1, fd );
	}
	fwrite( &END, sizeof( char ), 1, fd );

  zmq_close(responder);
  zmq_ctx_destroy(context); 
  VectorClean(vector); 
  return 0;
}
