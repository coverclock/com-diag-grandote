/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_THREAD_
#define _H_COM_DIAG_DESPERADO_THREAD_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include <pthread.h>

namespace com {
namespace diag {
namespace desperado {

/**
 * Thread implements a POSIX thread which can be associated with a thread of
 * control, that is, an independent context via which a processor may execute
 * instructions with other threads of control on the same processor
 * (serially via context switching) or on other processors (concurrently).
 */
class Thread {

public:

	/**
	 * This defines the prototype for a function that can be started in the
	 * new thread of control as an alternative to overriding the run method.
	 */
	typedef void * (Function)(void * data);

	/**
	 * Returns a reference to the Thread that represents the calling thread
	 * of control. Desperado guarantees that all calls to this class method
	 * will return the same Thread object when called by the same calling
	 * thread of control.
	 *
	 * @return a reference to the Thread representing the calling thread.
	 */
	static Thread & instance();

	/**
	 * Cause the calling thread of control to yield the processor to another
	 * thread of control that is ready to run if such a thread exists.
	 */
	static void yield();

	/**
	 * Cause the calling thread of control to exit.
	 *
	 * @param result becomes the final value of the thread of control.
	 */
	static void exit(void * result = 0);

	/**
	 * Return the POSIX thread identifier for the calling thread of control.
	 *
	 * @return the POSIX thread idenifier for the calling thread of control.
	 */
	static ::pthread_t self();

public:

	/***************************************************************************
	 * The class methods in this section are used to implement the C-linkage
	 * ABI to POSIX Threads and must necessarily be publicly accessible. Don't
	 * call them. (If C++ allowed friend functions with C-linkage this wouldn't
	 * be necessary.)
	 **************************************************************************/

	static void cleanup_mutex(void * arg);

	static void cleanup_thread(void * arg);

	static void * start_routine(void * arg);

protected:

	static void * dontcare;

	static Thread main;

	static ::pthread_key_t key;

	static int setupped;

	static int setup();

	static void * empty_function(void * context);

	bool running;

	bool notifying;

	bool canceling;

	bool joining;

	Function * function;

	void * context;

	void * final;

	::pthread_t identity;

	::pthread_mutex_t mutex;

	::pthread_cond_t condition;

public:

	/**
	 * Constructor. A Thread object is created, but its thread of control is
	 * not automatically started.
	 */
	explicit Thread();

	/**
	 * Destructor. Deleting a Thread while its thread of control is running is
	 * really a bad idea. The destructor tries to minimize the damange, but
	 * chances are you are looking at an imminent application crash.
	 */
	virtual ~Thread();

	/**
	 * Start a new thread of control which will be associated with this
	 * Thread object. A new thread of control can only be started when there
	 * is not one already running.
	 *
	 * @param implementation refers to a function that is executed in the new
	 *        thread of control as an alternative to overriding the run method.
	 * @param data is passed to the function in the new thread of control.
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int start(Function & implementation = empty_function, void * data = 0);

	/**
	 * Wait for the thread of control associated with this Thread to terminate.
	 * Unlike the POSIX thread join, multiple threads of control can join with
	 * the same Thread, even if the thread of control associated with this
	 * Thread isn't running, or has never been started. It is however an error
	 * for a thread of control to join with itself. The first thread of control
	 * to become unblocked in this method does an actual POSIX join on the
	 * terminating Thread; some thread implementations use this to clean up
	 * resources in the underlying platform before the parent process
	 * terminates.
	 *
	 * @param result refers to an optional variable into which the final value
	 *        of the thread of control associated with this Thread.
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int join(void * & result = dontcare);

	/**
	 * Notify the Thread. This merely sets the a flag in this Thread which can
	 * be interrogated by the notified method. Once a Thread is notified, it
	 * remains notified until it terminates and is re-started.
	 *
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int notify();

	/**
	 * Return the notification state of this Thread.
	 *
	 * @return true if this Thread has been notified, false otherwise.
	 */
	virtual bool notified();

	/**
	 * Get the POSIX thread identifier for the thread of control associated with
	 * this Thread. This value is undefined if the thread of control has
	 * never been started.
	 *
	 * @return the POSIX thread identifier associated with this Thread.
	 */
	::pthread_t getIdentity() { return identity; }

	/**
	 * Get the final value of the thread of control associated with this Thread.
	 * This value is undefined until the thread of control has terminated and,
	 * depending on how the thread terminated, has been joined.
	 *
	 * @return the final value of the thread of control.
	 */
	void * getFinal() { return final; }

protected:

	explicit Thread(::pthread_t id);

	/**
	 * A derived class may override this method to provide an implementation
	 * for the thread of control to execute with this Thread is started. This
	 * is an alternative to providing a function.
	 *
	 * @return the final value of the thread of control.
	 */
	virtual void * run();

private:

	void initialize();

    /**
     *  Copy constructor. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Thread(const Thread& that);

    /**
     *  Assignment operator. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
	Thread& operator=(const Thread& that);

public:

	/***************************************************************************
	 * You should endeavor not to use methods in this section. They are not your
	 * friends. Wackiness may ensue. They are provided for completeness. You
	 * have been warned.
	 **************************************************************************/

	/**
	 * Provide an explicit POSIX thread cancellation point for the calling
	 * thread of control. Using cancel semantics is a really bad idea for lots
	 * of reasons, but this method is provided for completeness.
	 */
	static void cancellable();

	/**
	 * Cancel the thread of control associated with this Thread. Once a Thread
	 * is canceled, it remains canceled until it completes and is re-started.
	 * Using cancel semantics is a really bad idea for lots of reasons, but
	 * this method is provided for completeness.
	 *
	 * @return 0 for success or an error number if an error occurred.
	 */
	virtual int cancel();

	/**
	 * Return true if this Thread has been cancelled. Using cancel
	 * semantics is a really bad idea for lots of reasons, but this method
	 * is provided for completeness.
	 *
	 * @return true if this Thread has been cancelled, false otherwise.
	 */
	virtual bool cancelled();

};

}
}
}

#endif
