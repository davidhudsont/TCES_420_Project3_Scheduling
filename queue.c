
#include <stdlib.h>
#include <stdio.h>
#include <queue.h>


int isEmpty(queue *q){
	return (q->qsize ==0);	
}	    

int isFull(queue *q){
	return (q->qsize ==q->qcapacity);	
}

void queue_init(queue *q,int capacity) {
	q->jobqueue = (job*)malloc(capacity*sizeof(job));
	q->qcapacity = capacity;
	q->head = 0;
	q->tail = capacity-1;
	q->qsize = 0;
}

void queue_delete(queue *q) {
	for (int i=0; i<q->qcapacity; i++){
		delete_job(&q->jobqueue[i]);
	}
	free(q->jobqueue);
	//free(q);
}
void enqueue(queue *q,job j) {
	if (isFull(q)) { return; } 
	q->tail = (q->tail + 1)%q->qcapacity;
	q->jobqueue[q->tail] = j;
	q->qsize++;
}

job dequeue(queue *q) {
	if (isEmpty(q)) {
		job j;
		j.job_id =-1;
		return j;
	}
	job ret= q->jobqueue[q->head];
	q->head = (q->head+1)%q->qcapacity;
	q->qsize--;
	return ret;
}
	    


