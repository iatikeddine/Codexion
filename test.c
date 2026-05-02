#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void*	routine(){
	printf("Test from Threads\n");
	sleep(3);
	printf("Ending Thread\n");
	return NULL;
}

int main(int argc, char **argv){
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_create(&t2, NULL, &routine, NULL);
	pthread_join(t1, NULL);
	pthread_join(t1, NULL);
	return 0;
}
