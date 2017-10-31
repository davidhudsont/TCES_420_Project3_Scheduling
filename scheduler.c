#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define qSIZE 4
#define jSIZE 4

typedef struct Job {
	int job_id; 
	int duration;
	int current_phase;
	int is_completed; 
	
}job; 

typedef struct Queue {
	
	job* jobqueue; 
	int head,tail;
}queue;


void enqueue(queue *a,job b) {
	for (int i=0; i<qSIZE; i++) {
		if (a->tail==4 && a->head!=a->tail) {
			printf("Queue is full!\n");
			return;
		}
		if (a->head==a->tail) {
			a->head =0;
			a->tail =0;
			a->jobqueue[i]=b;
			a->tail++;
			return;
		}
		if (i==a->tail) {
			a->jobqueue[i]=b;
			a->tail= a->tail+1;
			return;
		}
	}
}

job* dequeue(queue *a) {
	for (int i=0; i<qSIZE; i++) {
		if (a->head == a->tail && a->tail!=0) {
			a->head = 0;
		}
		if (i==a->head) {
			job *ret = &a->jobqueue[i];
			a->head++;
			return ret;
		}
	}
}

int queue_size(queue *a) {
	int quantity=0;
	for (int i=a->head; i<a->tail; i++) {
		quantity++;
	}
	return quantity;
}

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
	run.jobqueue = (job*)calloc(qSIZE,sizeof(job));
	io.jobqueue = (job*)calloc(qSIZE,sizeof(job));
	//done.jobqueue = (job*)malloc(qSIZE*sizeof(job));
	srand(time(NULL));
		
	for (int i=0; i<jSIZE; i++) {
		job* j = (job*)malloc(sizeof(job));
		int dur = rand()%5;
		init_job(j,i+1,dur,0,0);
		enqueue(run_ptr,*j);
	}
	for (int i=0; i<queue_size(run_ptr); i++) {
		printf("job_id: %d job_dur: %d\n", run.jobqueue[i].job_id,run.jobqueue[i].duration);
	}
	for (int i=0; i<jSIZE; i++) {
		job* j = dequeue(run_ptr);
		delay(j->duration);
		printf("Job Finished\n");
		enqueue(io_ptr,*j);
		printf("job_id: %d job_dur: %d\n", io.jobqueue[i].job_id,io.jobqueue[i].duration);
	}
	
	return 0;
	
	
	
	
}