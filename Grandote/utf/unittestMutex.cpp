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
 *  Implements the Mutex unit test.
 *
 *  @see    Mutex
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *	Since Desperado doesn't provide any threading mechanism, this can't really
 *	be tested here. Most of the functional testing for this class was done in
 *	the Hayloft unit test suite, uses the Desperado Mutex and CriticalSection
 *	classes and does implement threading (Thread) and also condition variables
 *	(Condition).
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/Mutex.h"
#include "com/diag/desperado/Mutex.h"
#include "com/diag/desperado/CriticalSection.h"
#include "com/diag/desperado/CriticalSection.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

static Mutex staticMutex;

static int level = 0;

CXXCAPI int unittestMutex(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;
    int status;
    bool disabled;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticMutex.show();

    Mutex mutex;

    printf("%s[%d]: mutex begin\n", __FILE__, __LINE__);

    mutex.show();
    if ((status = mutex.begin()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
    mutex.show();

    printf("%s[%d]: mutex end\n", __FILE__, __LINE__);

    mutex.show();
    if ((status = mutex.end()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
    mutex.show();

    printf("%s[%d]: mutex recursion\n", __FILE__, __LINE__);

    mutex.show();
    if ((status = mutex.begin()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
    mutex.show();
    if ((status = mutex.attempt()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
    mutex.show();
    if ((status = mutex.begin()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
    mutex.show();
    if ((status = mutex.end()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
    mutex.show();
    if ((status = mutex.end()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
    mutex.show();
    if ((status = mutex.end()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
    mutex.show();

    printf("%s[%d]: critical section\n", __FILE__, __LINE__);

    struct MyCriticalSection : public CriticalSection {
    	explicit MyCriticalSection(Mutex& mutexr, bool disable = true)
    		: CriticalSection(mutexr, disable)
    	{
    		++level;
    	}
    	virtual ~MyCriticalSection() {
    		--level;
    	}
    	int getStatus() { return this->status; }
    	bool getDisabled() { return this->disabled; }
    };

    {
        MyCriticalSection one(mutex);
        if ((status = one.getStatus()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
        if (!(disabled = one.getDisabled())) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, disabled, true); ++errors; }
        if (level != 1) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, level, 1); ++errors; }
        mutex.show();
        {
            MyCriticalSection two(mutex);
            if ((status = two.getStatus()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
            if (!(disabled = two.getDisabled())) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, disabled, true); ++errors; }
            if (level != 2) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, level, 2); ++errors; }
            mutex.show();
        }   
        if (level != 1) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, level, 1); ++errors; }
        mutex.show();
    }
    if (level != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, level, 0); ++errors; }
    mutex.show();

    printf("%s[%d]: critical section enabled\n", __FILE__, __LINE__);

    {
        MyCriticalSection one(mutex, false);
        if ((status = one.getStatus()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
        if ((disabled = one.getDisabled())) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, disabled, false); ++errors; }
        if (level != 1) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, level, 1); ++errors; }
        mutex.show();
        {
            MyCriticalSection two(mutex, false);
            if ((status = two.getStatus()) != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, status, 0); ++errors; }
            if ((disabled = two.getDisabled())) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, disabled, false); ++errors; }
            if (level != 2) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, level, 2); ++errors; }
            mutex.show();
        }
        if (level != 1) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, level, 1); ++errors; }
        mutex.show();
    }
    if (level != 0) { errorf("%s[%d]: %d!=%d!\n", __FILE__, __LINE__, level, 0); ++errors; }
    mutex.show();

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}

