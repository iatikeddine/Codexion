# include "../inc/codexion.h"

long    get_current_time()
{
    long    current_time;
    struct timeval  timevalue;

    gettimeofday(&timevalue, NULL);
    current_time = (timevalue.tv_sec * 1000) + (timevalue.tv_usec / 1000);

    return current_time;
}

void    take_dongle(t_dongle *dongle, t_coder *coder)
{
    t_request   request;
    long    time_left_to_burnout;
    long    current_time;

    pthread_mutex_lock(&dongle->lock);

    time_left_to_burnout = coder->args->time_to_burnout + coder->last_compile_time;
    request.time_left_to_burnout = time_left_to_burnout;
    request.arrival_time = get_current_time();
    request.coder = coder;
 
    push_request(&dongle->queue, request, coder->args->scheduler);

    while (dongle->is_available == 0 || get_current_time() < dongle->cooldown || dongle->queue.requests[0].coder != coder)
        pthread_cond_wait(&dongle->wait_cond, &dongle->lock);
    
    pop_request(&dongle->queue);
    dongle->is_available = 0;
    pthread_mutex_unlock(&dongle->lock);
}

void    drop_dongle(t_dongle *dongle, long cooldown_duration)
{
    long current_time;

    current_time = get_current_time();

    pthread_mutex_lock(&dongle->lock);
    dongle->is_available = 1;
    dongle->cooldown = current_time + cooldown_duration;
    pthread_cond_broadcast(&dongle->wait_cond);
    pthread_mutex_unlock(&dongle->lock);
}