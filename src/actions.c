# include "../inc/codexion.h"

void    take_dongle(t_dongle *dongle, t_coder *coder)
{
    t_request   request;
    long    time_to_burnout;

    pthread_mutex_lock(&dongle->lock);

    time_to_burnout = coder->args->time_to_burnout + coder->last_compile_time;
    request.time_left_to_burnout = time_to_burnout;
    request.arrival_time = get_current_time();
    request.coder = coder;

    push_request(&dongle->queue, request, coder->args->scheduler);

    while (dongle->is_available == 0 || dongle->queue.requests[0].coder != coder || get_current_time() < dongle->cooldown)
    {
        if (dongle->is_available == 1 && dongle->queue.requests[0].coder == coder && get_current_time() < dongle->cooldown)
        {
            struct timespec ts;
            ts.tv_sec  = dongle->cooldown / 1000;
            ts.tv_nsec = (dongle->cooldown % 1000) * 1000000;
            pthread_cond_timedwait(&dongle->wait_cond, &dongle->lock, &ts);
        }
        else
            pthread_cond_wait(&dongle->wait_cond, &dongle->lock);
    }
    pop_request(&dongle->queue);
    print_status("has taken a dongle", coder);
    dongle->is_available = 0;
    pthread_mutex_unlock(&dongle->lock);
}

void    drop_dongle(t_dongle *dongle, long cooldown_duration)
{
    pthread_mutex_lock(&dongle->lock);
    dongle->is_available = 1;
    dongle->cooldown = get_current_time() + cooldown_duration;
    pthread_cond_broadcast(&dongle->wait_cond);
    pthread_mutex_unlock(&dongle->lock);
}