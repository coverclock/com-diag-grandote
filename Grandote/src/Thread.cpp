/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/Thread.h"
#include "com/diag/desperado/MaskableLogger.h"
#include "com/diag/desperado/errno.h"

namespace com {
namespace diag {
namespace desperado {

/*******************************************************************************
 * CLASS VARIABLES
 ******************************************************************************/

void * Thread::dontcare = 0;

pthread_key_t Thread::key;

Thread Thread::main(::pthread_self());

int Thread::setupped = Thread::setup();

/*******************************************************************************
 * C ABI PROXY FUNCTIONS
 ******************************************************************************/

// These functions probably aren't necessary. But strictly speaking, class
// (static) methods in C++ don't necessarily have C linkage, and POSIX Threads
// is an API all based on C linkage. So the only really reliable and portable
// way to hook up C++ with POSIX Threads is to place a thin layer of C linkage
// functions in between the two.

extern "C" {

	static void cleanup_mutex_proxy(void * arg) {
		Thread::cleanup_mutex(arg);
	}

	static void cleanup_thread_proxy(void * arg) {
		Thread::cleanup_thread(arg);
	}

	static void * start_routine_proxy(void * arg) {
		return Thread::start_routine(arg);
	}

}

/*******************************************************************************
 * IMPLEMENTATION CLASS METHODS
 ******************************************************************************/

int Thread::setup() {
	int rc = ::pthread_key_create(&key, 0);
	if (rc == 0) {
		rc = ::pthread_setspecific(key, &main);
	}
	return rc;
}

void Thread::cleanup_mutex(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	::pthread_mutex_unlock(&that->mutex);
}

void Thread::cleanup_thread(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	if (that->running) {
		::pthread_mutex_lock(&that->mutex);
		that->running = false;
		::pthread_cond_broadcast(&that->condition);
		::pthread_mutex_unlock(&that->mutex);
	}
}

void * Thread::start_routine(void * arg) {
	Thread * that = static_cast<Thread *>(arg);
	int dontcare;
	::pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &dontcare);
	::pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &dontcare);
	::pthread_setspecific(key, that);
	pthread_cleanup_push(cleanup_thread_proxy, that);
	MaskableLogger::instance().debug("Thread@%p{0x%lx}: run{0x%lx}\n", that, pthread_self(), that->identity);
	that->final = that->run();
	pthread_cleanup_pop(!0);
	return that->final;
}

void * Thread::empty_function(void *) {
	return 0;
}

/*******************************************************************************
 * APPLICATION CLASS METHODS
 ******************************************************************************/

Thread & Thread::instance() {
	return *(static_cast<Thread *>(::pthread_getspecific(key)));
}

void Thread::yield() {
	::pthread_yield();
}

void Thread::exit(void * result) {
	Thread * that = &(Thread::instance());
	::pthread_mutex_lock(&that->mutex);
	pthread_cleanup_push(cleanup_mutex_proxy, that);
	that->final = result;
	pthread_cleanup_pop(!0);
	::pthread_exit(result);
}

::pthread_t Thread::self() {
	return ::pthread_self();
}

/*******************************************************************************
 * IMPLEMENTATION INSTANCE METHODS
 ******************************************************************************/

void Thread::initialize() {
	::pthread_mutex_init(&mutex, 0);
	::pthread_cond_init(&condition, 0);
}

Thread::Thread(::pthread_t id)
: running(true)
, notifying(false)
, canceling(false)
, joining(false)
, function(0)
, context(0)
, final(reinterpret_cast<void *>(~0))
, identity(id)
{
    initialize();
}

/*******************************************************************************
 * APPLICATION INSTANCE METHODS
 ******************************************************************************/

Thread::Thread()
: running(false)
, notifying(false)
, canceling(false)
, joining(true)
, function(0)
, context(0)
, final(reinterpret_cast<void *>(~0))
, identity(pthread_self())
{
    initialize();
}

// Unlike Java, there is no way to defer deleting this object if its thread is
// running. Wackiness ensues no whether the deleting thread is another thread
// or this thread itself. We try to do some stuff to minimize the damage, but
// we should consider treating this as a fatal error for the entire process.
Thread::~Thread() {
	bool self = false;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex_proxy, this);
	if (!running) {
		// Do nothing.
	} else if (!::pthread_equal(pthread_self(), identity)) {
		::pthread_cancel(identity);
		::pthread_cond_wait(&condition, &mutex);
	} else {
		running = false;
		::pthread_cond_broadcast(&condition);
		self = true;
	}
	pthread_cleanup_pop(!0);
	if (self) {
		::pthread_yield();
	}
	::pthread_cond_destroy(&condition);
	::pthread_mutex_destroy(&mutex);
	if (self) {
		::pthread_exit(reinterpret_cast<void*>(~0));
	}
}

int Thread::start(Function & implementation, void * data) {
	int rc;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex_proxy, this);
	if (!running) {
		running = true;
		notifying = false;
		joining = false;
		function = &implementation;
		context = data;
		final = reinterpret_cast<void *>(~0);
		rc = ::pthread_create(&identity, 0, start_routine_proxy, this);
		if (rc == 0) {
			MaskableLogger::instance().debug("Thread@%p{0x%lx}: start{0x%lx}\n", this, pthread_self(), identity);
		} else {
			running = false;
			joining = true;
		}
	} else {
		rc = EBUSY;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

int Thread::notify() {
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex_proxy, this);
	notifying = true;
	pthread_cleanup_pop(!0);
	MaskableLogger::instance().debug("Thread@%p{0x%lx}: notify{0x%lx}\n", this, pthread_self(), identity);
	return 0;
}

bool Thread::notified() {
	bool result;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex_proxy, this);
	result = notifying;
	pthread_cleanup_pop(!0);
	return result;
}

int Thread::join(void * & result) {
	int rc;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex_proxy, this);
	// It's okay to invoke a Desperado Thread join even if the thread of control
	// is not running. It is not okay for a thread of control to try to join
	// with itself.
	if (!running) {
		rc = 0;
	} else if (::pthread_equal(pthread_self(), identity)) {
		rc = EBUSY;
	} else {
		rc = ::pthread_cond_wait(&condition, &mutex);
	}
	// The first thread unblocked by the terminating thread does an actual
	// POSIX thread join operation. Some thread implementations depend on this
	// to clean up underlying resources in the platform before the parent
	// process terminates. We save the value given to us by the POSIX thread
	// join; for some code paths, it's the only way to get the final value of
	// the thread of control associated with this Thread. It also guarantees
	// that the thread of control has completely terminated from a POSIX POV.
	if (rc == 0) {
		if (!joining) {
			rc = ::pthread_join(identity, &final);
			if (rc == 0) {
				joining = true;
				MaskableLogger::instance().debug("Thread@%p{0x%lx}: join{0x%lx}\n", this, pthread_self(), identity);
			}
		}
	}
	// We have to check the return code again because it could have changed if
	// the join failed. The final value is only valid if everything succeeded.
	if (rc == 0) {
		result = final;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

void * Thread::run() {
	return (function != 0) ? (*function)(context) : 0;
}

/*******************************************************************************
 * DEPRECATED INSTANCE METHODS
 ******************************************************************************/

void Thread::cancellable() {
	::pthread_testcancel();
}

int Thread::cancel() {
	int rc;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex_proxy, this);
	if (!running) {
		rc = 0;
	} else if (!::pthread_equal(pthread_self(), identity)) {
		canceling = true;
		rc = ::pthread_cancel(identity);
		if (rc == 0) {
			MaskableLogger::instance().debug("Thread@%p{0x%lx}: cancel{0x%lx}\n", this, pthread_self(), identity);
		}
	} else {
		rc = EBUSY;
	}
	pthread_cleanup_pop(!0);
	return rc;
}

bool Thread::cancelled() {
	bool result;
	::pthread_mutex_lock(&mutex);
	pthread_cleanup_push(cleanup_mutex_proxy, this);
	result = canceling;
	pthread_cleanup_pop(!0);
	return result;
}

}
}
}
