#ifndef _COM_DIAG_DESPERADO_MUTEX_H_
#define _COM_DIAG_DESPERADO_MUTEX_H_

/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Desperadito library.
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    As a special exception, if other files instantiate templates or
    use macros or inline functions from this file, or you compile
    this file and link it with other works to produce a work based on
    this file, this file does not by itself cause the resulting work
    to be covered by the GNU Lesser General Public License. However
    the source code for this file must still be made available in
    accordance with the GNU Lesser General Public License.

    This exception does not invalidate any other reasons why a work
    based on this file might be covered by the GNU Lesser General
    Public License.

    Alternative commercial licensing terms are available from the copyright
    holder. Contact Digital Aggregates Corporation for more information.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place, Suite 330,
    Boston, MA 02111-1307 USA, or http://www.gnu.org/copyleft/lesser.txt.

******************************************************************************/


/**
 *  @file
 *
 *  Declares the Mutex class.
 *
 *  @see    Mutex
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include <pthread.h>
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a mutual exclusion lock, or mutex, for POSIX pthreads.
 *  The mutex allows recursion: if the same thread locks the mutex
 *  more than once without releasing it, the thread does not deadlock.
 *  However, begin() (lock) and end() (unlock) operations must be
 *  correctly bracketed.
 *
 *  @see    D. Butenhof, <I>Programming with POSIX Threads</I>,
 *          Addison-Wesley, 1997
 *
 *  @see    M. Mitchell et al., <I>Advanced Linux Programming</I>,
 *          NewRiders, 2001
 *
 *  @see    CriticalSection
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Mutex : public Object {

	friend class Condition;

public:

    /**
     *  Constructor.
     */
    explicit Mutex();

    /**
     *  Destructor.
     */
    virtual ~Mutex();

    /**
     *  Locks the mutex.
     *
     *  @return 0 for success or an error number otherwise.
     */
    virtual int begin();

    /**
     *  Tries to lock the mutex but returns false instead of blocking if the
     *  mutex is locked by another thread of control. (Not recommended for
     *  applications, but useful in unit testing.)
     *
     *  @return 0 for success or an error number otherwise.
     */
    virtual int attempt();

    /**
     *  Unlocks the mutex.
     *
     *  @return 0 for success or an error number otherwise.
     */
    virtual int end();

    /**
     *  Displays internal information about this object to the specified
     *  output object. Useful for debugging and troubleshooting.
     *
     *  @param  level   sets the verbosity of the output. What this means
     *                  is object dependent. However, the level is passed
     *                  from outer to inner objects this object calls the
     *                  show methods of its inherited or composited objects.
     *
     *  @param display  points to the output object to which output is
     *                  sent. If null (zero), the default platform output
     *                  object is used as the effective output object. The
     *                  effective output object is passed from outer to
     *                  inner objects as this object calls the show methods
     *                  of its inherited and composited objects.
     *
     *  @param  indent  specifies the level of indentation. One more than
     *                  this value is passed from outer to inner objects
     *                  as this object calls the show methods of its
     *                  inherited and composited objects.
     */
    virtual void show(int level = 0, Output* display = 0, int indent = 0) const;

protected:

    /**
     *  This is the POSIX pthread mutex attribute.
     */
    ::pthread_mutexattr_t mutexattr;

    /**
     *  This is the POSIX pthread mutex.
     */
    ::pthread_mutex_t mutex;

private:

    /**
     *  Copy constructor. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
    Mutex(const Mutex& that);

    /**
     *  Assignment operator. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
    Mutex& operator=(const Mutex& that);

};

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Mutex unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestMutex(void);
#endif


#endif
