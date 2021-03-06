/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_GRANDOTE_CONDITION_
#define _H_COM_DIAG_GRANDOTE_CONDITION_

/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Grandote.html<BR>
 */

#include <pthread.h>
#include "com/diag/grandote/types.h"

namespace com {
namespace diag {
namespace grandote {

class Mutex;

/**
 * Condition implements a POSIX thread condition variable. Condition variables
 * are a form of thread synchronization using a monitor-like mechanism. Threads
 * of control may wait until receiving a signal from another thread. Signals
 * are not persistent: if no thread is waiting on a Condition when a thread
 * signals the Condition, the signal does nothing. All waiting must be done
 * inside a critical section (as implemented by CriticalSection) that holds
 * a mutex (as implemented by Mutex). Signaling is not necessarily done
 * inside a critical section. All Condition signals are broadcast signals: all
 * waiting threads are woken up and queue for the critical section. As near as
 * I can tell, POSIX does not specify whether its condition variables are
 * Hoare-like (the signaling thread blocks) or Mesa-like (the signaling thread
 * does not block). I suspect the latter. Note that while it is not required
 * that an application signal while inside a critical section that holds a
 * mutex, there are very few synchronization patterns in which that would not
 * be a (possibly subtle) bug in the application.
 */
class Condition {

protected:

	::pthread_cond_t condition;

public:

	/**
	 * Constructor.
	 */
	explicit Condition();

	/**
	 * Destructor.
	 */
	virtual ~Condition();

	/**
	 * Use this as the wait() timeout if you want it to block indefinitely.
	 */
	static const ticks_t INFINITE = ~(ticks_t)0;

	/**
	 * Block waiting on this Condition inside a CriticalSection which has Mutex
	 * locked until all Threads blocked on this Condition are signaled.
	 *
	 * @param mutex is the mutex in whose critical section the wait is being
	 *        done.
	 * @param timeout is the relative timeout period in platform ticks; returns
	 *        ETIMEDOUT if the timeout expired.
	 * @return 0 for success or an error number of an error occurred.
	 */
	virtual int wait(Mutex & mutex, ticks_t timeout = INFINITE);

	/**
	 * Broadcast a signal and hence unblock all Threads waiting on this
	 * Condtion.
	 *
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int signal();

private:

    /**
     *  Copy constructor. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Condition(const Condition& that);

    /**
     *  Assignment operator. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Condition& operator=(const Condition& that);

};

}
}
}

#endif
