
#ifndef LIST_J_
#define LIST_J_

typedef struct Job {
	unsigned int job_id:10; 			// The job id
	unsigned int tasks:2;					// Number of task to be completed
	unsigned int task_type:1;			// The type of task 0(cpu) or 1(io)
	unsigned int current_task:2;					// The current task to be completed
	unsigned int task_counter:1; 	// The current number of tasks completed
	unsigned int is_completed:1;	// Is the job completed

	void (*next) 			(struct Job*); 	// Queue up the next task
	void (*complete) 	(struct Job*); 	// Complete the next task
	void (*destroy) 	(struct Job*);	// Destroy the job and free associated memory
	void (*print) 		(struct Job*);	// Print out the current state of Job
	
}job; 

job* create_job(int id);

void next_task(job *j);

void delete_job(job *j);

void complete_task(job *j);

void print_job(job *j);


#endif
