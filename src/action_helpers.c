# include "codexion.h"

static void	fill_request(t_request *request, t_coder *coder)
{
	long	time_to_burnout;

	pthread_mutex_lock(&coder->args->state_lock);
	time_to_burnout = coder->args->time_to_burnout + coder->last_compile_time;
	pthread_mutex_unlock(&coder->args->state_lock);
	request->time_left_to_burnout = time_to_burnout;
	request->arrival_time = get_current_time();
	request->coder = coder;
}

void	fill_requests(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder)
{

	t_request	request;

	pthread_mutex_lock(&dongle_1->lock);
	fill_request(&request, coder);
	push_request(&dongle_1->queue, request, coder->args->scheduler);
	pthread_mutex_unlock(&dongle_1->lock);

	pthread_mutex_lock(&dongle_2->lock);
	fill_request(&request, coder);
	push_request(&dongle_2->queue, request, coder->args->scheduler);
	pthread_mutex_unlock(&dongle_2->lock);
}

void	one_coder(t_coder *coder)
{
	long	time_to_burnout;

	time_to_burnout = get_current_time() + coder->args->dongle_cooldown;
	while (get_current_time() < time_to_burnout)
		usleep(1000);
}

int	both_ready(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder)
{
	int	left_ready;
	int	right_ready;

	left_ready = (dongle_1->is_available
		&& dongle_1->queue.requests[0].coder == coder
		&& get_current_time() >= dongle_1->cooldown);

	right_ready = (dongle_2->is_available &&
		dongle_2->queue.requests[0].coder == coder
		&& get_current_time() >= dongle_2->cooldown);
	
	if (right_ready && left_ready)
		return (1);
	return (0);
}

long	highest_cooldown(t_dongle *dongle_1, t_dongle *dongle_2)
{
	long	highest_cooldown;

	if (dongle_1->cooldown > dongle_2->cooldown)
		highest_cooldown = dongle_1->cooldown;
	else if (dongle_1->cooldown < dongle_2->cooldown)
		highest_cooldown = dongle_2->cooldown;
	else
		highest_cooldown = dongle_1->cooldown;

	return (highest_cooldown);
}