#include <stdio.h>
#include "threads.h"

int main(int argc, char *argv[]) {

	int runtime = 15;
	if (argc == 2) {
		runtime = atoi(argv[1]);
		printf("Run time %d\n", runtime);
	}
	
	schedule_init();
	
	// Job submission thread initialization
	schedule_thread_start();
	
	wait(runtime);
	stop = 1;
	
	//printf("Times UP!!!\n");
	// Join all threads
	schedule_thread_join();
	
	printf("# of Jobs Created: %d\n",counter-1);
	printf("# of Jobs Finished %d\n",job_counter);
	double percent = (double) job_counter/(counter-1)*100.0;
	printf("Percentage Complted %f%%\n",percent );
	printf("DONE!!!!!!!\n");
	
	schedule_destroy();
  printf("Press Enter to Close the Program");
	char x;
	scanf("%c",&x);
	return 0;
}
