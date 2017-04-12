/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_CRITICALSECTION_
#define _H_COM_DIAG_DESPERADO_CRITICALSECTION_

/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/CriticalSection.h"
#include "com/diag/desperado/Mutex.h"

namespace com {
namespace diag {
namespace desperado {

/**
 * CancellableCriticalSection locks a Mutex in its constructor and unlocks it in
 * its destructor. This allows you to implement a critical section protected by
 * a Mutex by using the C++ "Resource Allocation is Initialization" idiom. The
 * critical section is protected by the associated Mutex while an object of
 * this type is in scope. This class is mostly an alias for the Desperado
 * CriticalSection class. But unike Desperado CriticalSection, the default
 * behavior for Hayloft CriticalSection is to enable cancellation of the
 * constructing thread of control while an object of this type is in scope.
 * Instead, it relies on the C++ run time unwinding the stack to insure that
 * the Mutex is released. Having said that, using thread cancellation is a
 * really really bad idea for lots of really really good reasons. You can
 * disable cancellation by not using the default value for the second
 * constructor parameter, by using the Desperado base class, or by using the
 * similarly scope-controlled Uncancellable class. (I confess this is mostly
 * an experiment.)
 */
class CancellableCriticalSection : public CriticalSection {

public:

    /**
     *  Constructor.
     *
     * @param mutexr refers to a mutex object.
     * @param disable if true causes the calling thread to be uncancellable
     *                when an object of this type is in scope. To leave the
     *                calling thread cancellable is the default behavior.
     */
    explicit CancellableCriticalSection(Mutex& mutexr, bool disable = false)
    : CriticalSection(mutexr, disable)
    {}

};

}
}
}

#endif
