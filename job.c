
#include <stdio.h>
#include <job.h>


void init_job(job* j,int id,int cpu,int io, int phase) {
	j->job_id = id;
	j->cpu_duration = cpu;
	j->io_duration = io;
	j->current_phase = phase;
	return;
}
