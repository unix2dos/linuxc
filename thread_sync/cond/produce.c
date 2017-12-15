#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct list {
	struct list* next;
	int num;
};



struct list* head = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void* produce(void* arg) {
	while (1) {
		struct list* p = malloc(sizeof(struct list));
		pthread_mutex_lock(&mutex);
		p->next = head;
		p->num = rand()%1000+1;
		head = p;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		printf("produce %d\n", p->num);
		sleep(rand() % 5);
	}
}


void* cost(void* arg){
	while (1) {
		pthread_mutex_lock(&mutex);
		while (head == NULL) {
			pthread_cond_wait(&cond, &mutex);	
		}
		struct list* p = head;
		head = p->next;
		pthread_mutex_unlock(&mutex);
		printf("cost %d\n", p->num);
		free(p);
		sleep(rand() % 5);
	}

}


int main(int argc, char* argv[]) {
	srand(time(NULL));

	pthread_t c, p;
	pthread_create(&p, NULL, produce, NULL);
	pthread_create(&c, NULL, cost, NULL);

	pthread_join(p, NULL);
	pthread_join(c, NULL);

	return 0;
}
