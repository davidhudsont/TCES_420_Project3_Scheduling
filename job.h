#include <stdio.h>

#ifndef LIST_J_
#define LIST_J_

typedef struct Job {
	unsigned int job_id:10; 
	unsigned int tasks:2;
	unsigned int task_type:1;
	unsigned int task:2;
	unsigned int current_task:1; 
	unsigned int is_completed:1;

	void (*next) (struct Job*);
	void (*complete) (struct Job*);
	void (*destroy) (struct Job*);
	void (*print) (struct Job*);
	
}job; 

job* init_job(int id);

void next_task(job *j);
void delete_job(job *j);
void complete_task(job *j);
void print_job(job *j);


#endif
