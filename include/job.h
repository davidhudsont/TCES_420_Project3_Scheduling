#pragma once

#include <stdbool.h>
#include <stdint.h>

struct JOB
{
    enum TASK_TYPE
    {
        IO,
        CPU,
    };
    uint32_t job_id;      // The job id
    uint8_t tasks;        // Number of task to be completed
    TASK_TYPE task_type;  // The type of task io(0) or cpu(1)
    uint8_t current_task; // The current task to be completed
    uint8_t task_counter; // The current number of tasks completed
    bool is_completed;    // Is the job completed

    JOB(int id); // Create a new job

    void next_task(); // Queue up the next task

    void complete_task(); // Complete the next task

    void print_job(); // Print out the current state of Job
};
