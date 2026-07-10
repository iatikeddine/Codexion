# include "../inc/codexion.h"

static int check_simulation(t_coder *coder)
{
    // prevent race with monitor thread in case they oth check at the same time
    pthread_mutex_lock(&coder->args->state_lock);
    if (coder->args->ending_point == 't' || coder->number_of_compiles == coder->args->number_of_compiles_required)
    {
        pthread_mutex_unlock(&coder->args->state_lock);
        return 1;
    }
    pthread_mutex_unlock(&coder->args->state_lock);
    return 0;
}

static void    compiling(t_coder *coder)
{
    if (coder->coder_id % 2 == 0)
    {
        take_dongle(coder->right_dongle, coder);
        take_dongle(coder->left_dongle, coder);
    }
    else
    {
        take_dongle(coder->left_dongle, coder);
        take_dongle(coder->right_dongle, coder);
    }

    print_status("is compiling", coder);

    pthread_mutex_lock(&coder->args->state_lock);
    coder->last_compile_time = get_current_time();
    coder->number_of_compiles++;
    pthread_mutex_unlock(&coder->args->state_lock);
    usleep(coder->args->time_to_compile * 1000);
    drop_dongle(coder->left_dongle, coder->args->dongle_cooldown);
    drop_dongle(coder->right_dongle, coder->args->dongle_cooldown);
}

static void    debugging(t_coder *coder)
{
    print_status("is debugging", coder);
    usleep(coder->args->time_to_debug * 1000);
}

static void    refactoring(t_coder *coder)
{
    print_status("is refactoring", coder);
    usleep(coder->args->time_to_refactor * 1000);
}

void    *routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while(check_simulation(coder) == 0)
    {
        compiling(coder);
        if (check_simulation(coder) == 1)
            return NULL;
        debugging(coder);
        if (check_simulation(coder) == 1)
            return NULL;
        refactoring(coder);
    }

    return NULL;
}
