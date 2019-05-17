
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int isEmpty(QUEUE* q) { 
	return (q->qsize ==0);	
}	    

QUEUE* queue_init() { 
	QUEUE* q = (QUEUE*) malloc(sizeof(QUEUE));
	q->qsize = 0; 
	q->head = NULL;
	q->tail = NULL;
	return q;
}

JOB* dequeue_job(QUEUE* q) 
{
	if (isEmpty(q)) {
		return NULL;
	}
	JOB* j;
	NODE* tmp = q->head;
	q->head = q->head->next;
	j = node_destroy(tmp);
	q->qsize--;
	return j;
}

void queue_delete(QUEUE* q) { 
	while (q->qsize > 0) 
	{
		JOB* tmp = dequeue_job(q);
		delete_job(tmp);
	}
	free(q); 
}

void enqueue_job(QUEUE* q,JOB* j) {
	if (q->qsize==0)
	{
		q->head = init_node(j);
		q->tail = q->head;
	}
	else 
	{
		NODE *temp = init_node(j);
		q->tail->next = temp;
		q->tail = temp;
	}
	q->qsize++;
}

void print_queue(QUEUE* q) {
	NODE* current = q->head;
	while (current != NULL) {
		printf("");
	}
}
