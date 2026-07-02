#include "../inc/codexion.h"

void    *routine_monitoring(void *arg)
{
    int i;
    int count;
    long    last_compile_time;
    t_arguments *args;
    t_coder *coder;

    args = (t_arguments *)arg;

    while (get_current_time() < args->starting_time)
        usleep(100);
    
    while (1)
    {
        i = 0;
        while (i < args->number_of_coders)
        {
            coder = &args->coders[i];

            pthread_mutex_lock(&args->state_lock);
            last_compile_time = coder->last_compile_time;
            pthread_mutex_unlock(&args->state_lock);

            if (get_current_time() - last_compile_time >= args->time_to_burnout)
            {
                print_status("burned out", coder);
                pthread_mutex_lock(&args->state_lock);
                args->ending_point = 't';
                pthread_mutex_unlock(&args->state_lock);
                return NULL;
            }
            i++;
        }

        i = 0;
        count = 0;
        while (i < args->number_of_coders)
        {
            coder = &args->coders[i];

            pthread_mutex_lock(&args->state_lock);
            if (coder->number_of_compiles == args->number_of_compiles_required)
                count++;
            pthread_mutex_unlock(&args->state_lock);

            i++;
        }
        if (count == args->number_of_coders)
        {
            pthread_mutex_lock(&args->state_lock);
            args->ending_point = 't';
            pthread_mutex_unlock(&args->state_lock);
            return NULL;
        }
        usleep(1000);
    }
}