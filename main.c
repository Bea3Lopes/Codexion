#include "codexion.h"

// t_config -> stack (i know the size)
// t_simulation -> stack
// coders[N]-> heap (malloc)
// dongles[N]-> heap
int	main(int argc, char **argv)
{
	t_config config;
	t_simulation simulation;

	memset(&config, 0, sizeof(t_config)); //not needed allocate on stack

// 1. parse + validate args	-- fill t_config

	if (parser(argc, argv, &config) == 0)
		return 0;

        printf("coders   = %d\n", config.number_coders);
        printf("burnout  = %ld\n", config.time_to_burnout);
        printf("compile  = %ld\n", config.time_to_compile);
        printf("debug    = %ld\n", config.time_to_debug);
        printf("refactor = %ld\n", config.time_to_refactor);
        printf("required = %d\n", config.number_compiles_required);
        printf("cooldown = %ld\n", config.dongle_cooldown);
        printf("scheduler = %d\n", config.scheduler);

// 2. init simulation -- create t_simulation from t_config
	if (init_simulation(config, simulation) == 0)
		return 0;
// 3. initialize resources --
	// coders/dongles /scheduler, queue / synchronization objs

//4. start simulation -- 
	//create coder threads / create monitor thread
	//
// 5. wait for sim to finish -- join threads
	//
//6. cleanup --
	//destroy mutexes,conditons/ free coders, dongles, other allocations
	return 1;
}
