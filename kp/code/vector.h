#ifndef VECTOR_H
#define VECTOR_H

#define MSX 11
#define LSIZE 32

#include "data.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct {
	int id;
	long long debit ;
} Account;

typedef struct {
 	size_t used;
 	size_t capasity;
 	Account* vector;
} Vector;

Vector *VectorCreate (int size);
int VectorSearch (Vector *x, int ID);
int VectorAdd (Vector *x, int ID);
int VectorClean (Vector *x);

int CashToDebit (Vector *x, long long cash, int ID);

int Withdraw (Vector *x, long long cash, int ID);

int getStat(Vector *x, int ID, char* ans);
#endif