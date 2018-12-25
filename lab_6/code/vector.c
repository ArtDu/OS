#include "vector.h"

#define DEFAULT_DEBIT 0

Vector *VectorCreate(int size)
{
	if (size <= 0) {
		fprintf(stderr, "%s\n", "Uncorrect size!");
		return (NULL);
	}

	Vector *result = (Vector*) malloc(sizeof(Vector));
	if (result == NULL) {
		perror("Memory allocation error:");
		exit(1);
	}

	result->vector = (Account*) malloc(sizeof(Account) * size);
	if (result->vector == NULL) {
		perror("Memory allocation error:");
		exit(1);
	}

	result->capasity = size;
	result->used = 0;
	return result;
}

int VectorAdd(Vector *x, int ID){

	if (x->used == x->capasity) {
		x->capasity *= 2;
		x->vector = (Account*) realloc(x->vector, x->capasity * sizeof(Account));
		if (x->vector == NULL) {
			perror("Memory reallocation error:");
			exit(-1);
		}
	}

	x->vector[x->used].id = ID;
	x->vector[x->used].debit = DEFAULT_DEBIT;

	x->used++;
	return 1;
}

int VectorSearch(Vector *x, int ID){
	for (int i = 0; i < x->used; i++) 
		if (ID == x->vector[i].id) 
			return i;	
	return -1;
}

int CashToDebit(Vector *x, long long cash, int ID){
	if(cash < 0)
		return -1;
	int idx = VectorSearch(x, ID);
	if(idx<0)
		return -1;
	if (cash > 0)
		x->vector[idx].debit += cash;
	return 0;
}

int Withdraw(Vector *x, long long cash, int ID){
	if(cash < 0)
		return -1;
	int idx = VectorSearch(x, ID);
	if (idx<0)
		return -1;
	if (x->vector[idx].debit - cash >= 0) {
		x->vector[idx].debit -= cash;
		return 0;
	}

	return 2;
}


int getStat(Vector *x, int ID, char* ans){
	int idx = VectorSearch(x, ID);
	if(idx<0)
		return -1;
	sprintf(ans, "Id: %d\nDebit balace: %lld\n",
		x->vector[idx].id, x->vector[idx].debit);
	return 0;
}

int VectorClean(Vector *x){
	free(x->vector);
	free(x);
	return 0;
}


