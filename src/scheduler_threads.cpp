#include "scheduler_threads.h"
#include <stdio.h>

// Global Queue pointers
QUEUE *run_ptr;
QUEUE *io_ptr;
QUEUE *done_ptr;

// Run Queue locks
std::mutex ready_full;
std::mutex ready_empty;
std::mutex ready_lock;

// IO Queue locks
std::mutex io_full;
std::mutex io_empty;
std::mutex io_lock;

// Finished Queue locks
std::mutex finished_full;
std::mutex finished_empty;
std::mutex finished_lock;

std::mutex counter_lock;
std::mutex job_counter_lock;

// Job Book keeping
unsigned int counter;
unsigned int job_counter;

// Stops scheduler
int stop;

// Threads
std::vector<std::thread> job_submission;
std::vector<std::thread> cpu;
std::vector<std::thread> io;

int wait(int time)
{
    std::this_thread::sleep_for(std::chrono::seconds(time));
    return 1;
}

void stopScheduler()
{
    stop = 1;
}

int getCounter()
{
    return counter;
}

int getJobCounter()
{
    return job_counter;
}

void schedule_init(void)
{
    counter = 1;
    job_counter = 0;
    stop = 0;

    run_ptr = new QUEUE();
    io_ptr = new QUEUE();
    done_ptr = new QUEUE();
}

// Start job_submission, cpu and io threads
void schedule_thread_start(void)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        job_submission.push_back(std::thread(job_submission_thread, i));
    }
    // CPU thread initialization
    for (i = 0; i < 8; i++)
    {
        cpu.push_back(std::thread(cpu_thread, i));
    }
    // IO thread initialization
    for (i = 0; i < 4; i++)
    {
        io.push_back(std::thread(io_thread, i));
    }
}

// Join the job_submission, cpu and io threads
void schedule_thread_join(void)
{
    for (int i = 0; i < 4; i++)
    {
        job_submission[i].join();
    }
    for (int i = 0; i < 8; i++)
    {
        cpu[i].join();
    }
    for (int i = 0; i < 4; i++)
    {
        io[i].join();
    }
}

void schedule_destroy(void)
{
    delete run_ptr;
    delete io_ptr;
    delete done_ptr;
}

void job_submission_thread(int id)
{
    int thread = id;
    int t;
    while (stop != 1)
    {
        // Producer
        counter_lock.lock();
        counter++;
        run_ptr->enqueue_job(counter);
        printf("Job: #%02d -> Run  Queue, JOB Thread: #%d\n", counter, thread);
        counter_lock.unlock();

        // Consumer
        t = 0;
        while (t != 1)
        {
            finished_lock.lock();
            JOB *ja = done_ptr->dequeue_job();
            finished_lock.unlock();
            if (ja != NULL)
            {
                job_counter_lock.lock();
                job_counter++;
                job_counter_lock.unlock();
                printf("Job: #%02d -> Job  Freed, JOB Thread: #%d\n", ja->job_id, thread);
                delete ja;
            }
            t = wait(4);
        }
    }
    printf("JOB Thread: #%d is exiting\n", thread);
}

void cpu_thread(int id)
{
    long thread = id;
    while (stop != true)
    {
        ready_lock.lock();
        JOB *cpu = run_ptr->dequeue_job();
        ready_lock.unlock();
        if (cpu != NULL)
        {
            if (cpu->task_type == JOB::TASK_TYPE::CPU)
            {
                cpu->complete_task();
            }
            if (cpu->is_completed == true)
            {
                printf("Job: #%02d -> Done Queue, CPU Thread: #%d\n", cpu->job_id, thread);
                finished_lock.lock();
                done_ptr->requeue_job(cpu);
                finished_lock.unlock();
            }
            else if (cpu->task_type == JOB::TASK_TYPE::CPU)
            {
                printf("Job: #%02d -> Run  Queue, CPU Thread: #%d\n", cpu->job_id, thread);
                ready_lock.lock();
                run_ptr->requeue_job(cpu);
                ready_lock.unlock();
            }
            else
            {
                printf("Job: #%02d -> IO   Queue, CPU Thread: #%d\n", cpu->job_id, thread);
                io_lock.lock();
                io_ptr->requeue_job(cpu);
                io_lock.unlock();
            }
        }
    }
    printf("CPU Thread: #%d is exiting\n", thread);
}

void io_thread(int id)
{
    int thread = id;
    while (stop != true)
    {
        io_lock.lock();
        JOB *io = io_ptr->dequeue_job();
        io_lock.unlock();
        if (io != NULL)
        {
            if (io->task_type == JOB::TASK_TYPE::IO)
            {
                io->complete_task();
            }
            if (io->is_completed == true)
            {
                printf("Job: #%02d -> Done Queue, IO  Thread: #%d\n", io->job_id, thread);
                finished_lock.lock();
                done_ptr->requeue_job(io);
                finished_lock.unlock();
            }
            else if (io->task_type == JOB::TASK_TYPE::IO)
            {
                printf("Job: #%02d -> Run  Queue, IO  Thread: #%d\n", io->job_id, thread);
                ready_lock.lock();
                run_ptr->requeue_job(io);
                ready_lock.unlock();
            }
            else
            {
                printf("Job: #%02d -> IO   Queue, IO  Thread: #%d\n", io->job_id, thread);
                io_lock.lock();
                io_ptr->requeue_job(io);
                io_lock.unlock();
            }
        }
    }
    printf("IO Thread: #%d is exiting\n", thread);
}