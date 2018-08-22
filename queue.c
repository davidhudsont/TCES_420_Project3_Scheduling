
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int isEmpty(queue *q){ return (q->qsize ==0);	}	    

queue* queue_init() { 
	queue *q = (queue*) malloc(sizeof(queue));
	q->qsize = 0; 
	q->head = NULL;
	q->tail = NULL;
	q->enqueue = insert;
	q->destroy = queue_delete;
	q->dequeue = removejob;
	return q;
}

void queue_delete(queue *q) { 
	while (q->qsize > 0) {
		job *tmp = q->dequeue(q);
		tmp->destroy(tmp);
	}
	// printf("Free\n");
	free(q); 
}

void insert(queue *q,job *j) {
	if (q->qsize==0){
		q->head = init_node(j);
		q->tail = q->head;
	}
	else {
		node *temp = init_node(j);
		q->tail->next = temp;
		q->tail = temp;
	}
    	q->qsize++;
}

job* removejob(queue *q) {
	if (isEmpty(q)) {
		return NULL;
	}
    	job* j;
    	node *tmp = q->head;
    	q->head = q->head->next;
    	j = tmp->destroy(tmp);
    	q->qsize--;
    	return j;
}

