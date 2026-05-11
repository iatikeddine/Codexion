#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t fuel_mutex;
pthread_cond_t cond_fuel;
int	fuel = 0;


void	*fuel_filling(void *arg)
{
	int	i;

	i = 0;
	while(i < 5)
	{
		pthread_mutex_lock(&fuel_mutex);
		fuel += 60;
		printf("filling fuel... %d\n", fuel);
		pthread_mutex_unlock(&fuel_mutex);
		pthread_cond_broadcast(&cond_fuel);
		sleep(1);
		i++;
	}
	return NULL;
}

void	*car(void *arg)
{
	pthread_mutex_lock(&fuel_mutex);
	while (fuel < 40)
	{
		printf("No fuel waiting...\n");
		pthread_cond_wait(&cond_fuel, &fuel_mutex);
	}
	fuel -= 40;
	printf("got fuel now left... %d\n", fuel);
	pthread_mutex_unlock(&fuel_mutex);
	return NULL;
}

int main()
{
	pthread_t th[2];
	int	i;

	i = 0;
	pthread_mutex_init(&fuel_mutex, NULL);
	pthread_cond_init(&cond_fuel, NULL);
	while(i < 5)
	{
		if (i == 4)
		{
			if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0)
			{
				fprintf(stderr, "Error: mutex failed\n");
				return 1;
			}
		}
		else
		{
			if (pthread_create(&th[i], NULL, &car, NULL) != 0)
			{
				fprintf(stderr, "Error: mutex car failed\n");
				return 2;
			}
		}
		i++;
	}
	i = 0;
	while (i < 5)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			fprintf(stderr , "Error: join failed\n");
			return 3;
		}
		i++;
	}
	pthread_mutex_destroy(&fuel_mutex);
	pthread_cond_destroy(&cond_fuel);
	return 0;
 Signaling for condition variables (pthread_cond_signal vs pthread_cond_broadcast) }
