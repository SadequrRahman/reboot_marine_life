/*
 * File:   Thread.h
 * Author: Mohammad Sadequr Rahman
 *	
 *
 */
#include "Thread.h"
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


Thread::Thread() :
	started_(false)
{
}

Thread::~Thread()
{
	WaitStopped();
}

void *Thread::PthreadCallRun(void *tobject) {
	reinterpret_cast<Thread*>(tobject)->Run();
	return NULL;
}

void Thread::WaitStopped() {
	if (!started_) return;
	int result = pthread_join(thread_, NULL);
	if (result != 0) {
		fprintf(stderr, "err code: %d %s\n", result, strerror(result));
	}
	started_ = false;
}

void Thread::Start(int priority, uint32_t affinity_mask) {
	assert(!started_);  // Did you call WaitStopped() ?
	pthread_create(&thread_, NULL, &PthreadCallRun, this);

	if (priority > 0) {
		struct sched_param p;
		p.sched_priority = priority;
		pthread_setschedparam(thread_, SCHED_FIFO, &p);
	}

	if (affinity_mask != 0) {
		cpu_set_t cpu_mask;
		CPU_ZERO(&cpu_mask);
		for (int i = 0; i < 32; ++i) {
			if ((affinity_mask & (1 << i)) != 0) {
				CPU_SET(i, &cpu_mask);
			}
		}
		pthread_setaffinity_np(thread_, sizeof(cpu_mask), &cpu_mask);
	}

	started_ = true;
}

void Thread::Stop(void)
{
	if (started_) {
		pthread_cancel(thread_);
		WaitStopped();
	}
}
