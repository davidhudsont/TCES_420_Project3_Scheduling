#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <job.h>



void init_job(job* j,int id) {
	srand(time(NULL));
	j->is_completed = 0;
	j->current_phase = 0;
	j->job_id = id;
	j->tasks = rand()%4+2;
	j->phases = (int **)malloc(sizeof(int*)*2);
	j->phases[0] = malloc(j->tasks*sizeof(int));
	j->phases[1] = malloc(j->tasks*sizeof(int));
	for (int col=0; col<j->tasks; col++) {
			int phase_type = rand()%2;
			if (phase_type ==1) {
				j->phases[1][col] = 1;
				j->phases[0][col] = rand()%4+7;
			}
			else {
				j->phases[1][col] = 0;
				j->phases[0][col] = rand()%4+1;
			}
		}
	
	return;
}

void free_job(job* j){
    free(j->phases[0]);
    free(j->phases[1]);
    free(j->phases);
}