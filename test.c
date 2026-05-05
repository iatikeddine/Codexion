#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void	*my_turn(void *arg){
	int *v = (int *)arg;
	int i = 0;
	while(i < 8){
		sleep(1);
		printf("My turn %d %d\n", i, *v);
		i++;
		(*v)++;
	}
	return NULL;
}

void	your_turn(){
	int i = 0;
	while(i < 5){
		sleep(1);
		printf("your turn %d\n", i);
		i++;
	}
}

int main(){
	pthread_t t1;
	printf("%lu\n", t1);
	int v = 0;
	pthread_create(&t1, NULL, my_turn, &v);
	your_turn();
	pthread_join(t1, NULL);
}
