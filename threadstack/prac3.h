#include <pthread.h>

#define N_THREADS 10
#define MAX 10
#define T_MAX 100000

struct my_data {
	int val;
	char name[60];
};

void *coge_incrementa_deja();
