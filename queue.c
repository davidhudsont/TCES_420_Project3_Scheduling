
#include <stdlib.h>
#include <stdio.h>
#include <queue.h>


void enqueue(queue *q,job j) {
	struct Node *newNode;
    newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = j;
    newNode->next = NULL;
    if(queue->head == NULL);
        queue->head = newNode;
        queue->tail = newNode;
    else{
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    printf("enqueue is Success!!!\n");
    /*for (int i=0; i<q->qsize; i++) {
		if (q->tail==q->qsize && q->head!=q->tail) {
			printf("Queue is full!\n");
			return;
		}
		if (q->head==q->tail) {
			q->head =0;
			q->tail =0;
			q->jobqueue[i]=j;
			q->tail++;
			return;
		}
		if (i==q->tail) {
			q->jobqueue[i]=j;
			q->tail++;
			return;
		}
	}*/
}

job* dequeue(queue *q) {
	if(queue->head == NULL){
        printf("\nQueue is Empty!!!\n");
        return NULL;
    }
    struct Node *temp = queue->head;
    printf("\n element: %d\n",  queue->head->data);
    queue->head = queue->head->next;
    if( queue->head == NULL){
        q->tail = NULL;
    }
    return temp->data;
    
    
    /*for (int i=0; i<q->qsize; i++) {
		if (q->head == q->tail && q->tail!=0) {
			q->head = 0;
		}
		if (i==q->head) {
			job *ret = &q->jobqueue[i];
			q->head++;
			return ret;
		}
	}*/
}


void queue_init(queue *q,int size) {
	q->head = NULL;
	q->tail = NULL;
}
