#include <zmq.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <unistd.h>
#include <sys/types.h>
#include "data.h"


void menu() {
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("1) Add cash to debit account    \n");
  printf("2) Withdraw cash                \n");
  printf("3) Account statistics           \n");
  printf("4) Send money                   \n");
  printf("0) Exit                         \n");  
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

int main(int argc, char *argv[]) {

	Message message;
	int act = 0;
	char ans[256];

	void* context = zmq_ctx_new();
	void* server = zmq_socket(context, ZMQ_REQ);

	ans[0]='\0';
 	strcat(ans,"tcp://localhost:");
 	if(argc==2)
  		strcat(ans,argv[1]);
  	else
  		strcat(ans,"4040");
	int rc = zmq_connect(server, ans);
      
	if (rc != 0) {
		perror("zmq_connect");    
		zmq_close(server);
		zmq_ctx_destroy(context);    
		exit(1);
	}  
  
	printf("Enter client id:\n");
	scanf("%d", &message.clientId);
	message.amount = 0;
	message.receiverId = 0;
	message.action = INIT;
	zmq_msg_t clientReq;
	zmq_msg_init_size(&clientReq, sizeof(Message));
	memcpy(zmq_msg_data(&clientReq), &message, sizeof(Message));
	zmq_msg_send(&clientReq, server, ZMQ_DONTWAIT);
	zmq_msg_close(&clientReq);
	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_msg_recv(&reply, server, 0);
	strcpy(ans, (char*)zmq_msg_data(&reply));
  
	if (strcmp(ans, "OK") == 0) {
		printf("\nWelcome~\n");
	} else if (strcmp(ans, "ERROR") == 0) {
		printf("Sorry! Server returned error\n");
		exit(1);
	} else {
		printf("Sorry! Bad server answer. Try again later\n");
		exit(1);
	}

	zmq_msg_close(&reply);

	for(;;) {
		menu();
		if (scanf("%d", &act) == EOF) {
			act = 0;
		} 
		



		if (act == 1) {
			printf("Write sum you want to add: ");
			scanf("%lld", &(message.amount));
			message.action = ADD_DEBIT;
			message.receiverId = 0;

		} else if (act == 2) {
			printf("Write sum you want to withdraw: ");
			scanf("%lld", &(message.amount));
			message.action = WTHDRAW_DEBIT;
			message.receiverId = 0;
		
		} else if (act == 3) {
			message.amount = 0;
			message.action = STAT;
			message.receiverId = 0;
		
		} else if (act == 4) {
			printf("Write sum you want to send: ");
			scanf("%lld", &(message.amount));
			printf("Who you want to send to: ");
			scanf("%d", &(message.receiverId));
			message.action = SEND;
		
		} else if (act == 0) {
		  message.amount = 0;
		  message.action = CLIENT_OFF;
		  message.receiverId = 0;
		  zmq_msg_init_size(&clientReq, sizeof(Message));
		  memcpy(zmq_msg_data(&clientReq), &message, sizeof(Message));

		  printf("Sending...\n");
		  zmq_msg_send(&clientReq, server, 0);
		  zmq_msg_close(&clientReq);

			break;

		} else {
			printf("Try more...\n");
			continue;
		}

		zmq_msg_init_size(&clientReq, sizeof(Message));
		memcpy(zmq_msg_data(&clientReq), &message, sizeof(Message));

		printf("Sending...\n");
		zmq_msg_send(&clientReq, server, 0);
		zmq_msg_close(&clientReq);
		zmq_msg_init(&reply);
		zmq_msg_recv(&reply, server, 0);

		strcpy(ans, (char*)zmq_msg_data(&reply));
		if (strcmp(ans, "ERROR") == 0) {
	  		printf("Error occured\n");
		} else {
	 		printf("%s\n",ans);
		}
		zmq_msg_close(&reply);

	}
  zmq_close(server);
  zmq_ctx_destroy(context);
  return 0;
}


