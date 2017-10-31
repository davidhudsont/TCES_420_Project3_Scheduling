#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <queue.h>

#define qSIZE 4
#define jSIZE 4

typedef struct Job {
	int job_id; 
	int duration;
	int current_phase;
	int is_completed; 
	
}job; 

void init_job(job* j,int id,int time,int crt_phase, int is_comp) {
	j->job_id = id;
	j->duration = time;
	j->current_phase = crt_phase;
	j->is_completed = is_comp;
	return;
}
queue run;
queue io;
queue done;
queue *run_ptr = &run;
queue *io_ptr = &io;
queue *done_ptr = &done;

void delay(int time){
	int t = clock();
	while ((float)((clock()-t)/CLOCKS_PER_SEC) <time) {}
}

int main() {
	queue_init(run_ptr,qSIZE);
	
	return 0;
}
