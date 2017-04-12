#ifndef _COM_DIAG_DESPERADO_CRITICALSECTION_H_
#define _COM_DIAG_DESPERADO_CRITICALSECTION_H_

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
 *  Declares the CriticalSection class.
 *
 *  @see    CriticalSection
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Mutex.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Given a Mutex, locks it upon construction, and unlocks it
 *  upon destruction, allowing a Mutex to be automatically
 *  locked and unlocked as the critical section goes in and out
 *  of scope. This exploits the "Resource Acquisition is Initialization"
 *  idiom.
 *
 *  @see    B. Stroustrup, <I>The C++ Programming Language</I>,
 *          3rd edition, pp 366-367, "resource acquisition is
 *          initialization"
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class CriticalSection {

public:

    /**
     *  Constructor.
     *
     *  @param  mutexr	refers to a mutex object.
     *
     *  @oaram	disable	if true causes the calling thread to be uncancellable
     *  				when an object of this type is in scope. This is the
     *  				default behavior.
     */
    explicit CriticalSection(Mutex& mutexr, bool disable = true);

    /**
     *  Destructor.
     */
    virtual ~CriticalSection();

protected:

    /**
     *  This is a reference to the mutex.
     */
    Mutex& mutex;

    /**
     *	This is true if cancellation is blocked while the mutex is locked. Its
     *	value is only valid if the mutex is locked.
     */
    bool disabled;

    /**
     *  This is the prior enable/disable state of the thread that initially
     *  constructed an object of this type. Its value is only valid if the
     *  thread was made uncancellable by the constructor.
     */
    int state;

    /**
     * This is the return code, zero or an error number, returned by pthreads.
     * Used for unit testing.
     */
    int status;

private:

    /**
     *  Copy constructor. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
    CriticalSection(const CriticalSection& that);

    /**
     *  Assignment operator. POISONED.
     *
     *  @param that refers to an R-value object of this type.
     */
    CriticalSection& operator=(const CriticalSection& that);

};

#include "com/diag/desperado/End.h"


#endif
