/*
 * Synchronization primitives.
 * See synch.h for specifications of the functions.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>
#include <vm.h>

/////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *namearg, int initial_count)
{
	struct semaphore *sem;

	assert(initial_count >= 0);

	sem = kmalloc(sizeof(struct semaphore));	//sem dynamically allocated
	if (sem == NULL) {
		return NULL;
	}

	sem->name = kstrdup(namearg);	//string dupaicate. sets sem's name to namearg(function parameter)
	if (sem->name == NULL) {
		kfree(sem);
		return NULL;
	}

	sem->count = initial_count;
	return sem;
}

void
sem_destroy(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	
	/* From spl.h,
	* splhigh()    sets spl to the highest value, disabling all interrupts.
        * spl0()       sets spl to 0, enabling all interrupts.
        * splx(s)      sets spl to S, enabling whatever state S represents.
        *
        * All three return the old interrupt state
        */
        
	spl = splhigh();
	
	/* From, threah.h, thread_hassleepers does this:
	*
 	* Return nonzero if there are any threads sleeping on the specified
 	* address. Meant only for diagnostic purposes.
 	*/
 	
	assert(thread_hassleepers(sem)==0); 
	splx(spl);

	/*
	 * Note: while someone could theoretically start sleeping on
	 * the semaphore after the above test but before we free it,
	 * if they're going to do that, they can just as easily wait
	 * a bit and start sleeping on the semaphore after it's been
	 * freed. Consequently, there's not a whole lot of point in 
	 * including the kfrees in the splhigh block, so we don't.
	 */

	kfree(sem->name);
	kfree(sem);	//sem points to dynamically alocated memory and freed here
}

void 
P(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the P without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();	//disable interrupts
	
	while (sem->count==0) {	//blocks other threads
		thread_sleep(sem);
	}
	assert(sem->count>0);
	sem->count--;
	splx(spl);	//enable interrupts
}

void
V(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	
	spl = splhigh();	//disble interrupts
	sem->count++;
	
	assert(sem->count>0);	//if true continue
	thread_wakeup(sem);	//wake up all threads sleeping on this semaphore
	splx(spl);		//enable interupts
}

////////////////////////////////////////////////////////////
//
// Lock.

struct lock *
lock_create(const char *name)
{
	struct lock *lock;

	lock = kmalloc(sizeof(struct lock));
	if (lock == NULL) {
		return NULL;
	}

	lock->name = kstrdup(name);
	
	if (lock->name == NULL) {
		kfree(lock);
		return NULL;
	}
	
	// add stuff here as needed
	lock->held = 0;//newly added, held in synch.h lock structure
	
	return lock;
}

void
lock_destroy(struct lock *lock)
{
	assert(lock != NULL);

	// add stuff here as needed
	
	kfree(lock->name);
	kfree(lock);
}



void
lock_acquire(struct lock *lock)
{
	// Write this
	assert(lock!=NULL);
	//assert(in_interrupt==0);	//if in interrupt then error
	int spl;
	spl = splhigh();	//disble interrupts
	
	 //lock_acquire  code
	 int locker = 0;
	 if(lock->held == 0)//lock->held initally set to zero
	 {
	 	lock->held = 1;
	 	locker = 1;
	 }
	 
	 while(lock->held == 1 && locker == 0)//only threads which did not lock this will enter here
	 {	
	 	thread_sleep(lock);	//wake up once lock is released
	 }

	 lock->held = 1;//after lock released, want new thread to hold lock
	  
	 splx(spl);//enable interrupts
	 
	(void)lock;//suppress warning until code gets written
	
}

void
lock_release(struct lock *lock)
{
	// Write this
	assert(lock!=NULL);
	
	//assert(in_interrupt==0);	//if in interrupt then error
	
	int spl;
	spl = splhigh();	//disble interrupts
	 
	 //lock_release code
	lock->held = 0;
	thread_wakeup(lock);	//wake up sleeping threads which were made to sleep in thread_sleep(lock)
	
	splx(spl);	//enable interrupts
	
	(void)lock;  // suppress warning until code gets written
	
	
}

int
lock_do_i_hold(struct lock *lock)
{
	// Write this
	
	
	(void)lock;  // suppress warning until code gets written

}

////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
	struct cv *cv;

	cv = kmalloc(sizeof(struct cv));
	if (cv == NULL) {
		return NULL;
	}

	cv->name = kstrdup(name);
	if (cv->name==NULL) {
		kfree(cv);
		return NULL;
	}
	
	// add stuff here as needed
	
	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	// add stuff here as needed
	
	kfree(cv->name);
	kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)	//need to fix
{
	int spl;
	
	assert(lock != NULL);
	
	lock_release(lock);
	spl = splhigh();
	thread_sleep(cv);	
	splx(spl);	//added - reenable interrupts
	lock_acquire(lock);
	
	
}

void
cv_signal(struct cv *cv, struct lock *lock)	//bit left
{
	// Write this
	int spl;
	spl = splhigh();  //added - disable interrupts
	thread_wakeup_one(cv); //wake up one of the threads sleeping on this cv
	splx(spl);	//added - re-enable interrupts
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	//Added below:
	int spl;
	spl = splhigh();  //added - disable interrupts
	thread_wakeup(cv); //wake up all threads sleeping on this cv
	splx(spl);	//added - re-enable interrupts
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}


//--------------------------------------------------my lock functions----------------------------------------------------------------
struct lock *
my_lock_create(void)
{
	struct lock *lock;

	paddr_t pa;
	vaddr_t va;
	pa = ram_stealmem(2);
	if (pa == 0)
	{
		return NULL;
	}
	va = PADDR_TO_KVADDR(pa);
	lock = (struct lock*) va;
	
	// add stuff here as needed
	lock->held = 0;//newly added, held in synch.h lock structure
	return lock;
}
