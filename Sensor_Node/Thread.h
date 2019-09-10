/*
 * File:   Thread.h
 * Author: Mohammad Sadequr Rahman
 *	
 *	This file provide a wrapper class of pthread
 *  as well as mutex. After extenting need to implement 
 *  Run function which is the pthread entry function and 
 *  will start execution after calling Start().  After calling
 *  Stop() method, need to call WaitStopped() to clear succesfully 
 *  cancel the thread.
 */
#ifndef __THREAD__
#define __THREAD__

#include <stdint.h>
#include <pthread.h>

class Mutex;
class MutexLock;

class Thread
{
public:
	Thread();
	// The destructor waits for Run() to return so make sure it does.
	virtual ~Thread();
	// Wait for the Run() method to return.
	void WaitStopped();
	// Start thread. If realtime_priority is > 0, then this will be a
	// thread with SCHED_FIFO and the given priority.
	// If cpu_affinity is set !=, chooses the given bitmask of CPUs
	// this thread should have an affinity to.
	// On a Raspberry Pi 1, this doesn't matter, as there is only one core,
	// Raspberry Pi 2 can has 4 cores, so any combination of (1<<0) .. (1<<3) is
	// valid.
	virtual void Start(int realtime_priority = 0, uint32_t cpu_affinity_mask = 0);
	// stop thread
	void Stop(void);
	// Override this.
	virtual void Run() = 0;
private:
	static void *PthreadCallRun(void *tobject);
	bool started_;
	pthread_t thread_;
};

// Non-recursive Mutex.
class Mutex {
public:
	Mutex() { pthread_mutex_init(&mutex_, NULL); }
	~Mutex() { pthread_mutex_destroy(&mutex_); }
	void Lock() { pthread_mutex_lock(&mutex_); }
	void Unlock() { pthread_mutex_unlock(&mutex_); }
	void WaitOn(pthread_cond_t *cond) { pthread_cond_wait(cond, &mutex_); }

private:
	pthread_mutex_t mutex_;
};

// Useful RAII wrapper around mutex.
class MutexLock {
public:
	MutexLock(Mutex *m) : mutex_(m) { mutex_->Lock(); }
	~MutexLock() { mutex_->Unlock(); }
private:
	Mutex *const mutex_;
};

#endif /* __THREAD__ */