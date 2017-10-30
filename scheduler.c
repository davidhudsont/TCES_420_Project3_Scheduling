

#include <stdlib.h>
#include <stdio.h>

#define SIZE 4

typedef struct Job {
	int job_id; // 4

    int NR_PHASES;  // 4

	int current_phase; // 4

	// Phase types: 1 = CPU phase; 2 = IO phase

	//int phasetype_and_duration[NR_PHASES][NR_PHASES];

	int is_completed; // 4
	
}job; // 4+4+4+4=16

typedef struct Queue {
	//int* runqueue;
	job* jobqueue; 
	int head,tail;
}queue;
/*
int enqueue(queue run,int x) {
	for (int i=0; i<SIZE; i++) {
		if (run.runqueue[i]==0) {
			run.runqueue[i] = x;
			return x;
		}
		else {
			
		}
	}
	return 0;
}
int dequeue(queue run) {
	int first = run.runqueue[0];
	for (int i=0; i<SIZE; i++) {
		if (i>=SIZE-1) {
			run.runqueue[i] = 0;
			break;
		}
		run.runqueue[i] = run.runqueue[i+1];
	}
	return first;
}
*/

void enqueue(queue *a,job b) {
	for (int i=0; i<SIZE; i++) {
		if (a->tail==4 && a->head!=a->tail) {
			printf("Queue is full!\n");
			return;
		}
		if (a->head==a->tail) {
			a->head =0;
			a->tail =0;
			a->jobqueue[i]=b;
			a->tail++;
			printf("tail: %d, b.job_id: %d, a->jobqueue[i]: %d\n",a->tail,b.job_id,a->jobqueue[i].job_id);
			return;
		}
		if (i==a->tail) {
			a->jobqueue[i]=b;
			a->tail= a->tail+1;
			printf("tail: %d, b.job_id: %d, a->jobqueue[i]: %d\n",a->tail,b.job_id,a->jobqueue[i].job_id);
			return;
		}
	}
}

job dequeue(queue *a) {
	for (int i=0; i<SIZE; i++) {
		if (a->head == a->tail && a->tail!=0) {
			a->head = 0;
		}
		if (i==a->head) {
			job ret = a->jobqueue[i];
			a->head++;
			return ret;
		}
	}
}

int size(queue *a) {
	int quantity=0;
	for (int i=a->head; i<a->tail; i++) {
		quantity++;
	}
	return quantity;
}

int main() {
	queue run;
	job job1;
	run.jobqueue = (job*)malloc(SIZE*sizeof(job));
	printf("%d\n",sizeof(job));
	job1.job_id = 42;
	job job2;
	job2.job_id = 23;
	job job3;
	job3.job_id = 33;
	job job4;
	job4.job_id = 1;
	job job5;
	job5.job_id = 3;
	
	run.head = 0;
	run.tail = 0;
	queue *p;
	p = &run;
	enqueue(p,job1);
	enqueue(p,job2);
	enqueue(p,job3);
	enqueue(p,job4);
	enqueue(p,job5);
	for (int i=0; i<run.tail; i++) {
		printf("job_id: %d\n", run.jobqueue[i].job_id);
	}
	printf("qsize: %d\n",size(p));
	printf("tail: %d head %d\n",run.tail,run.head);
	dequeue(p);
	dequeue(p);
	dequeue(p);
	dequeue(p);
	printf("tail: %d head %d\n",run.tail,run.head);
	printf("qsize: %d\n",size(p));
	enqueue(p,job5);
	for (int i=0; i<run.tail; i++) {
		printf("job_id: %d\n", run.jobqueue[i].job_id);
	}
	printf("qsize: %d\n",size(p));
	printf("tail: %d head %d\n",run.tail,run.head);
	return 0;
	
	
	
	
}