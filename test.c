#include <sys/time.h>
#include <stdio.h>

int main()
{
    struct timeval the_time;
    gettimeofday(&the_time, NULL);

    long    mili = (long)the_time.tv_sec * 1000;
    long    micro = (long)the_time.tv_usec / 1000;

    printf("%ld\n", mili);
    printf("%ld\n", micro);
    printf("%ld\n", mili + micro);
}

