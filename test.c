#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

size_t	test = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void	*say_hello()
{
	size_t	i = 0;
	while(i < 1000000)
	{
		pthread_mutex_lock(&mutex);
		test++;
		i++;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main()
{
	pthread_t th[5];

	int	i = 0;
	while (i < 5)
	{
		if (pthread_create(th + i, NULL, &say_hello, NULL) != 0)
			return 1;
		i++;
	}
	i = 0;
	while (i < 5)
	{
		if (pthread_join(th[i], NULL) != 0)
			return 2;
		i++;
	}
	printf("%lu", test);
}
