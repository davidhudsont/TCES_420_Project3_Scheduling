
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int isEmpty(queue *q){ return (q->qsize ==0);	}	    

void queue_init(queue *q) { q->qsize = 0; }

void queue_delete(queue *q) { free(q); }

void enqueue(queue *q,job *a) {
    	node *temp = (node*)malloc(sizeof(node));
    	temp->j = a;
    	temp->next = NULL;
	if (q->qsize==0){
		q->head = temp;
		q->tail = temp;
	}
	else {
		q->tail->next = temp;
		q->tail = temp;
	}
    	q->qsize++;
}

job* dequeue(queue *q) {
	if (isEmpty(q)) {
		return NULL;
	}
    	job* js = q->head->j;
    	node* temp = q->head;
    	temp->j = NULL;
    	q->head = q->head->next;
    	q->qsize--;
    	free(temp);
    	return js;
}

