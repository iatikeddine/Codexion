#include "../inc/codexion.h"

static void    fifo_policy(t_heap *queue)
{
    if (queue->requests[1].arrival_time < queue->requests[0].arrival_time)
    {
        t_request temp;
        temp = queue->requests[0];
        queue->requests[0] = queue->requests[1];
        queue->requests[1] = temp;
    }
}

static void    edf_policy(t_heap *queue)
{
    if (queue->requests[1].time_left_to_burnout < queue->requests[0].time_left_to_burnout)
    {
        t_request temp;
        temp = queue->requests[0];
        queue->requests[0] = queue->requests[1];
        queue->requests[1] = temp;
    }
    else if (queue->requests[1].time_left_to_burnout == queue->requests[0].time_left_to_burnout)
        fifo_policy(queue);
}

void    push_request(t_heap *queue, t_request req, char scheduler_type)
{
    if (queue->current_size == 0)
    {
        queue->requests[0] = req;
        queue->current_size += 1;
    }
    else
    {
        queue->requests[1] = req;
        queue->current_size += 1;
    
        if (scheduler_type == 'F')
            fifo_policy(queue);

        else if (scheduler_type == 'E')
            edf_policy(queue);
    }
}

void    pop_request(t_heap *queue)
{
    if (queue->current_size == 1)
        queue->current_size = 0;

    else if (queue->current_size == 2)
    {
        queue->requests[0] = queue->requests[1];
        queue->current_size = 1;
    }
}
