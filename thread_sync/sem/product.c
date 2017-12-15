#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

#define NUM 5
int queue[NUM];
sem_t blank_num, product_num;



void* produce(void* arg) {
	int n = 0;
	while (1) {
		sem_wait(&blank_num);	
		queue[n] = rand()%1000+1;
		printf("produce %d\n", queue[n]);
		sem_post(&product_num);

		n = (n+1)%NUM;	
		sleep(rand()%5);
	}
}


void* cost(void* arg) {
	int n = 0;
	while (1) {
		sem_wait(&product_num);	
		printf("cost %d\n", queue[n]);
		queue[n] = 0;
		sem_post(&blank_num);
		n = (n+1)%NUM;	
		sleep(rand()%5);
	}
}



int main(int argc, char* argv[]) {

	srand(time(NULL));
	sem_init(&blank_num, 0, NUM);
	sem_init(&product_num, 0, 0);

	pthread_t c, p;
	pthread_create(&p, NULL, produce, NULL);			
	pthread_create(&c, NULL, cost, NULL);			
	pthread_join(p, NULL);
	pthread_join(c, NULL);

	sem_destroy(&blank_num);
	sem_destroy(&product_num);

	return 0;
}
