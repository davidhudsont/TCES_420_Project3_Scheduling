#include <stdbool.h>
#include <stdint.h>

#ifndef __JOB_H__
#define __JOB_H__


#define IO_TASK() 	(rand()%4+3)
#define CPU_TASK() 	(rand()%4+1)
#define TASK_GEN()	(rand()%2)
#define TASK_AMT()	(rand()%3+1)

typedef enum {
	IO,
	CPU,
} TASK_TYPE;

typedef struct job {
	uint32_t 	job_id; 			// The job id
	uint8_t 	tasks;				// Number of task to be completed
	TASK_TYPE 	task_type;			// The type of task io(0) or cpu(1)
	uint8_t 	current_task;		// The current task to be completed
	uint8_t 	task_counter; 		// The current number of tasks completed
	bool 		is_completed;		// Is the job completed

} JOB; 

JOB* create_job(int id);  // Create a new job

void next_task(JOB *j); // Queue up the next task

void delete_job(JOB *j); // Destroy the job and free associated memory

void complete_task(JOB *j); // Complete the next task

void print_job(JOB *j); // Print out the current state of Job


#endif
