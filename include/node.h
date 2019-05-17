#include <stdio.h>
#include <stdlib.h>
#include "job.h"

#ifndef __NODE_H__   
#define __NODE_H__

typedef struct node {
	JOB* j;
	struct node* next;
} NODE;

NODE* init_node(JOB *j);
JOB* node_destroy(NODE *n);

#endif