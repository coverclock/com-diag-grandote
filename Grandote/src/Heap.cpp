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
 *  Implements the Heap class.
 *
 *  @see    Heap
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/exceptions.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


inline size_t words(size_t bytes) {
    return ((bytes + sizeof(Heap::Alignment) - 1) /
            sizeof(Heap::Alignment)) + 1;
}


inline size_t bytes(size_t words) {
    return words * sizeof(Heap::Alignment);
}


//
//  Constructor.
//
Heap::Heap(Output* po) :
    ou(po),
    tracing(false),
    total(0),
    current(0),
    successes(0),
    failures(0),
    frees(0),
    nulls(0)
{
}


//
//  Constructor.
//
Heap::Heap(Output& ro) :
    ou(&ro),
    tracing(false),
    total(0),
    current(0),
    successes(0),
    failures(0),
    frees(0),
    nulls(0)
{
}


//
//  Destructor.
//
Heap::~Heap() {
}


//
//  Returns current output object.
//
Output& Heap::output() const {
    return this->ou ? *this->ou : Platform::instance().error();
}


//
//  Allocate a block of memory.
//
void* Heap::malloc(size_t size) {
    Print tracef(this->output());

    if (this->tracing) {
        tracef("Heap[%p]::malloc(%lu)\n", this, size);
    }

    Alignment* nptr;
    size_t dimension = words(size);

    try {
        nptr = new Alignment [dimension];
    } catch (...) {
        nptr = 0;
    }

    if (0 != nptr) {
        *(nptr++) = size;
        size_t nsize = bytes(dimension);
        this->total += nsize;
        this->current += nsize;
        ++this->successes;
    } else {
        ++this->failures;
    }

    if (this->tracing) {
        tracef("Heap[%p]::malloc(%lu)=%p\n", this, size, nptr);
    }

    if (0 == nptr) {
        errno = ENOMEM;
    }

    return nptr;
}


//
//  Free a block of memory.
//
void Heap::free(void* ptr) {
    Print tracef(this->output());

    if (this->tracing) {
        tracef("Heap[%p]::free(%p)\n", this, ptr);
    }

    Alignment* optr = static_cast<Alignment*>(ptr);

    if (0 != optr) {
        size_t osize = *(--optr);
        size_t nsize = bytes(words(osize));
        this->current -= nsize;
        delete [] optr;
        ++this->frees;
    } else {
        ++this->nulls;
    }

}


//
//  Reallocate a block of memory.
//
//  POSIX is ambiguous as to what happens when both ptr is null and
//  size is zero. We choose to mimic what Linux 2.6 does, which is
//  to act as if ptr is null and size is not zero.
//
void* Heap::realloc(void* ptr, size_t size) {
    Print tracef(this->output());

    if (this->tracing) {
        tracef("Heap[%p]::realloc(%p,%lu)\n", this, ptr, size);
    }

    void* nptr = 0;

    if (0 == ptr) {
        nptr = this->malloc(size);
    } else if (0 == size) {
        this->free(ptr);
    } else {
        size_t osize = this->size(ptr);
        if (osize != size) {
            nptr = this->malloc(size);
            if (0 != nptr) {
                std::memcpy(nptr, ptr, (osize < size) ? osize : size);
                this->free(ptr);
            }
        } else {
            nptr = ptr;
        }
   }

    if (this->tracing) {
        tracef("Heap[%p]::realloc(%p,%lu)=%p\n", this, ptr, size, nptr);
    }

    if ((0 != size) && (0 == nptr)) {
        errno = ENOMEM;
    }

    return nptr;
}


//
//  Allocate an array and zero it out.
//
void* Heap::calloc(size_t nmemb, size_t size) {
    Print tracef(this->output());

    if (this->tracing) {
        tracef("Heap[%p]::calloc(%lu,%lu)\n", this, nmemb, size);
    }

    size_t nsize = nmemb * size;
    void* nptr = this->malloc(nsize);

    if (0 != nptr) {
        memset(nptr, 0, nsize);
    }

    if (this->tracing) {
        tracef("Heap[%p]::calloc(%lu,%lu)=%zx\n", this, nmemb, size, nptr);
    }

    if (0 == nptr) {
        errno = ENOMEM;
    }

    return nptr;
}


//
//  Return the size of a previously allocated memory block.
//
size_t Heap::size(void* ptr) {
    Print tracef(this->output());

    if (this->tracing) {
        tracef("Heap[%o]::size(%p)\n", this, ptr);
    }

    Alignment* optr = static_cast<Alignment*>(ptr);
    size_t osize = 0;

    if (0 != optr) {
        osize = *(optr - 1);
    }

    if (this->tracing) {
        tracef("Heap[%p]::size(%p)=%lu\n", this, ptr, osize);
    }

    return osize;
}


//
//  Show this object on the output object.
//
void Heap::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s ou=%p\n", sp, this->ou);
    if (this->ou) {
        this->ou->show(level, display, indent + 2);
    }
    printf("%s tracing=%d\n", sp, this->tracing);
    printf("%s total=%u\n", sp, this->total);
    printf("%s current=%u\n", sp, this->current);
    printf("%s successes=%u\n", sp, this->successes);
    printf("%s failures=%u\n", sp, this->failures);
    printf("%s frees=%u\n", sp, this->frees);
    printf("%s nulls=%u\n", sp, this->nulls);
}


#include "com/diag/desperado/End.h"
