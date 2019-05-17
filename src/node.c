#include <stdlib.h>
#include <stdio.h>
#include "node.h"

NODE* init_node(JOB *j) {
	NODE *n = (NODE*)malloc(sizeof(NODE));
	n->j = j;
	n->next = NULL;
	return n;
}

JOB* node_destroy(NODE *n) {
	JOB *j 	= n->j;
	n->j 	= NULL;
	n->next = NULL;
	free(n);
	return j;
}


