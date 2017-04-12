/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the Mutex class.
 *
 *  @see    Mutex
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/Mutex.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
Mutex::Mutex()
{
    ::pthread_mutexattr_init(&(this->mutexattr));
    ::pthread_mutexattr_settype(&(this->mutexattr), PTHREAD_MUTEX_RECURSIVE);
    ::pthread_mutex_init(&(this->mutex), &(this->mutexattr));
}


//
//  Destructor.
//
Mutex::~Mutex() {
	::pthread_mutex_destroy(&(this->mutex));
	::pthread_mutexattr_destroy(&(this->mutexattr));
}


//
//  Lock.
//
int Mutex::begin() {
	return ::pthread_mutex_lock(&(this->mutex));
}


//
//  Try lock.
//
int Mutex::attempt() {
	return ::pthread_mutex_trylock(&(this->mutex));
}


//
//  Unlock.
//
int Mutex::end() {
	return ::pthread_mutex_unlock(&(this->mutex));
}


//
//  Show this object on the output object.
//
void Mutex::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    Dump dump(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s mutex:\n", sp);
    dump.words(&(this->mutex), sizeof(this->mutex), false, 0, indent + 2);
    printf("%s mutexattr:\n", sp);
    dump.words(&(this->mutexattr), sizeof(this->mutexattr), false, 0,
        indent + 2);
}


#include "com/diag/desperado/End.h"
