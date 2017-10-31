

#include <stdio.h>

typedef struct Job {
	int job_id; 
	int cpu_duration;
	int io_duration;
	int current_phase; 
}job; 

void init_job(job* j,int id,int cpu,int io, int phase);
