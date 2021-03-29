#include <stdio.h>
#include "scheduler_threads.h"

int main(int argc, char *argv[])
{

    int runtime = 15;
    bool close_console;
    if (argc == 3)
        close_console = atoi(argv[2]);
    if (argc == 2)
    {
        close_console = 0;
        runtime = atoi(argv[1]);
        printf("Run time %d\n", runtime);
    }

    schedule_init();

    // Job submission thread initialization
    srand(time(0));
    schedule_thread_start();

    wait(runtime);
    stopScheduler();

    //printf("Times UP!!!\n");
    // Join all threads
    schedule_thread_join();

    int counter = getCounter();
    int job_counter = getJobCounter();
    printf("# of Jobs Created: %d\n", counter - 1);
    printf("# of Jobs Finished %d\n", job_counter);
    double percent = (double)job_counter / (counter - 1) * 100.0;
    printf("Percentage Completed %f%%\n", percent);
    printf("DONE!!!!!!!\n");

    schedule_destroy();

    if (close_console)
    {
        printf("Press Enter to Close the Program");
        char x;
        scanf("%c", &x);
    }
    return 0;
}
