#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "job.h"

JOB* create_job(int id) 
{
	JOB *j = (JOB*) malloc(sizeof(JOB));
	j->is_completed = 0;
	j->task_counter = 0;
	j->job_id = id;
	j->tasks = TASK_AMT();
	j->task_type = TASK_GEN();
	if (j->task_type == IO) 
		j->current_task = IO_TASK();
	else if (j->task_type == CPU) 
		j->current_task = CPU_TASK();
	return j;
}

void delete_job(JOB *j) 
{
	// printf("Free\n");
	free(j);
}
/**
 * Queue up the next task within JOB struct
 * 
 * parameter[in] = JOB *j
 */
void next_task(JOB *j) 
{
	j->task_counter++;
	if (j->task_counter == j->tasks)
		j->is_completed = 1;
	else 
	{
		j->task_type = TASK_GEN();
		if (j->task_type == IO) 
			j->current_task = IO_TASK();
		else if (j->task_type == CPU) 
			j->current_task = CPU_TASK();
	}
}

/**
 * Completes the current task and 
 * queues up the next task.
 * 
 * parameters[in] = JOB *j
 * 
 */
void complete_task(JOB *j) 
{
	if (j->task_counter < j->tasks)
	{
		sleep(j->current_task);
		next_task(j); 
	}
	else 
		printf("Job is Finished!\n");
}

void print_job(JOB *j) 
{
	printf("Job ID: %d, Current Task: %d Job Complete: %d #ofTasks: %d\n",
		j->job_id,j->task_counter,j->is_completed,j->tasks);
}



