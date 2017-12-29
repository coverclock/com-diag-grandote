/**
 * @file
 *
 * Copyright 2011-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Grandote.html<BR>
 */

#include "com/diag/grandote/Condition.h"
#include "com/diag/grandote/Mutex.h"
#include "com/diag/grandote/Ticks.h"
#include "com/diag/grandote/errno.h"
#include <time.h>

namespace com {
namespace diag {
namespace grandote {

Condition::Condition()
{
	::pthread_cond_init(&condition, 0);
}

Condition::~Condition() {
	::pthread_cond_broadcast(&condition);
	::pthread_yield();
	::pthread_cond_destroy(&condition);
}

int Condition::wait(Mutex & mutex, ticks_t timeout) {
	int rc;
	struct timespec now;
	Ticks ticks;
	uint64_t seconds;
	uint32_t nanoseconds;
	if (timeout == INFINITE) {
		rc = ::pthread_cond_wait(&condition, &mutex.mutex);
	} else if (::clock_gettime(CLOCK_REALTIME, &now) < 0) {
		rc = errno;
	} else {
		ticks.seconds(timeout, seconds, nanoseconds);
		timeout = now.tv_nsec;
		timeout += nanoseconds;
		now.tv_nsec = timeout % 1000000000;
		now.tv_sec += seconds;
		now.tv_sec += timeout / 1000000000;
		rc = ::pthread_cond_timedwait(&condition, &mutex.mutex, &now);
	}
	return rc;
}

int Condition::signal() {
	return ::pthread_cond_broadcast(&condition);
}

}
}
}
