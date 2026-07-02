#include "../inc/codexion.h"

void    print_status(char *str, t_coder *coder)
{
    long    timestamp;

    pthread_mutex_lock(&coder->args->print_lock);

    pthread_mutex_lock(&coder->args->state_lock);
    if (coder->args->ending_point == 't')
    {
        pthread_mutex_unlock(&coder->args->state_lock);
        pthread_mutex_unlock(&coder->args->print_lock);
        return;
    }
    pthread_mutex_unlock(&coder->args->state_lock);

    timestamp = get_current_time() - coder->args->starting_time;
    printf("%ld %d %s\n", timestamp, coder->coder_id, str);

    pthread_mutex_unlock(&coder->args->print_lock);
}

long    get_current_time()
{
    long    current_time;
    struct timeval  timevalue;

    gettimeofday(&timevalue, NULL);
    current_time = (timevalue.tv_sec * 1000) + (timevalue.tv_usec / 1000);

    return current_time;
}