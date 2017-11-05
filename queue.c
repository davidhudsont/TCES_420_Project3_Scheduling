
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
	q->jobqueue = (job*)calloc(capacity,sizeof(job));
	q->qcapacity = capacity;
	q->head = 0;
	q->tail = capacity-1;
	q->qsize = 0;
}

void enqueue(queue *q,job j) {
	if (isFull(q)) { return; } 
	q->tail = (q->tail + 1)%q->qcapacity;
	q->jobqueue[q->tail] = j;
	q->qsize++;
}

job dequeue(queue *q) {
	if (isEmpty(q)) { return;}
	job *ret = &q->jobqueue[q->head];
	q->head = (q->head+1)%q->qcapacity;
	q->qsize--;
	return *ret;
}
	    


