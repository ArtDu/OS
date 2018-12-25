#ifndef DATA_H
#define DATA_H

typedef enum {
    INIT, ADD_DEBIT,
    WTHDRAW_DEBIT,
    STAT, SEND, CLIENT_OFF
} RequestToken;

typedef struct {    
  RequestToken action;
  int clientId;
  int receiverId;
  long long amount;
} Message ;

#endif