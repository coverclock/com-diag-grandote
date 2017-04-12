/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/Uncancellable.h"
#include <pthread.h>

namespace com {
namespace diag {
namespace desperado {

Uncancellable::Uncancellable()
{
    ::pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);
}

Uncancellable::~Uncancellable() {
	int dontcare;
    ::pthread_setcancelstate(state, &dontcare);
}

}
}
}
