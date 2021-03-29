#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <thread>
#include <vector>
#include <stdint.h>
#include <mutex>
#include "queue.h"

int wait(int time);

void stopScheduler();

int getCounter();
int getJobCounter();

void schedule_init(void);

void schedule_thread_start(void);

void schedule_thread_join(void);

void schedule_destroy(void);

void job_submission_thread(int id);

void cpu_thread(int id);

void io_thread(int id);
