

#include <stdio.h>

typedef struct Job {
	int job_id; 
	int** phases;
	int tasks;
	int current_phase; 
}job; 

void init_job(job* j,int id);
