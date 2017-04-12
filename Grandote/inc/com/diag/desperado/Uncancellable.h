/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_UNCANCELLABLE_
#define _H_COM_DIAG_DESPERADO_UNCANCELLABLE_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

namespace com {
namespace diag {
namespace desperado {

/**
 * Uncancellable causes the current thread to become uncancellable while an
 * object of this type is in scope. You shouldn't be using cancel semantics
 * anyway, for lots of reasons. But if are, and you need to suppress
 * cancellation for the duration of a block of code, this object will do that
 * for you.
 */
class Uncancellable {

protected:

    int state;

public:

    /**
     *  Constructor.
     */
    explicit Uncancellable();

    /**
     *  Destructor.
     */
    virtual ~Uncancellable();

};

}
}
}

#endif
