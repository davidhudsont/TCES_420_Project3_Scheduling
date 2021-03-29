#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include "job.h"

#define IO_TASK() (rand() % 4 + 3)
#define CPU_TASK() (rand() % 4 + 1)
#define TASK_GEN() (rand() % 2)
#define TASK_AMT() (rand() % 3 + 1)

JOB::JOB(int id)
    : is_completed(0),
      task_counter(0),
      job_id(id)
{
    tasks = TASK_AMT();
    task_type = (TASK_TYPE)TASK_GEN();
    if (task_type == IO)
        current_task = IO_TASK();
    else if (task_type == CPU)
        current_task = CPU_TASK();
}

void JOB::next_task()
{
    task_counter++;
    if (task_counter == tasks)
    {
        is_completed = 1;
    }
    else
    {
        task_type = (TASK_TYPE)TASK_GEN();
        if (task_type == IO)
            current_task = IO_TASK();
        else if (task_type == CPU)
            current_task = CPU_TASK();
    }
}

void JOB::complete_task()
{
    if (task_counter < tasks)
    {
        std::this_thread::sleep_for(std::chrono::seconds(current_task));
        next_task();
    }
    else
    {
        printf("Job is Finished!\n");
    }
}

void JOB::print_job()
{
    printf("Job ID: %d, Current Task: %d Job Complete: %d #ofTasks: %d\n",
           job_id, task_counter, is_completed, tasks);
}
