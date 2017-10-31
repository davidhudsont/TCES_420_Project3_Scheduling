

#include <stdio.h>

typedef struct Job {
	int job_id; 
	int duration;
	int current_phase;
	int is_completed; 
	
}job; 

void init_job(job* j,int id,int time,int crt_phase, int is_comp);
