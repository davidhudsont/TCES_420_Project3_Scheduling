
#include <stdlib.h>
#include <stdio.h>
#include <queue.h>


int isEmpty(queue *q){
	return (q->qsize ==0);	
}	    


void queue_init(queue *q) {
	//q->head = (node*)malloc(sizeof(node));
	//q->head->next = NULL;
	//q->head->j = NULL;
	//q->head = NULL;
	q->qsize = 0;
	
}

void queue_delete(queue *q) {
	
	free(q);
}
void enqueue(queue *q,job *a) {
	if (q->qsize==0){
		q->head = (node*)malloc(sizeof(node));
		q->head->j = a;
		q->qsize++;
		q->tail = q->head;
	}
	else {
		node *temp = (node*)malloc(sizeof(node));
		q->tail->next = temp;
		temp->j = a;
		q->tail = temp;
	}
}

job* dequeue(queue *q) {
	if (isEmpty(q)) {
		return NULL;
	}
	else {
	node* js = q->head;
	q->head = q->head->next;
	q->qsize--;
	free(&js->next);
	return js->j;
	}
}

