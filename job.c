
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <job.h>



void init_job(job* j,int id) {
	srand(time(NULL));
	j->is_completed = 0;
	j->job_id = id;
	j->tasks = rand()%4+2;
	j->current_phase = 0;
	j->phases = (int **)malloc(j->tasks*sizeof(int*));
	for (int i=0; i<2; i++) {
		j->phases[i] = (int*)malloc(2*sizeof(int));
	}
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
