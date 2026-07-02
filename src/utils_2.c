#include "../inc/codexion.h"

void	free_all(t_arguments *args)
{
	if (args->coders)
		free(args->coders);
	if (args->dongles)
		free(args->dongles);
}