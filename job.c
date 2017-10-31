
#include <stdio.h>
#include <job.h>


void init_job(job* j,int id,int time,int crt_phase, int is_comp) {
	j->job_id = id;
	j->duration = time;
	j->current_phase = crt_phase;
	j->is_completed = is_comp;
	return;
}
