#ifndef _COM_DIAG_DESPERADO_MALLOC_H_
#define _COM_DIAG_DESPERADO_MALLOC_H_

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
 *  Defines replacements for malloc(3) using the platform heap object.
 *
 *  This can be included from either a C or C++ translation unit.
 *
 *  Your compiler may give you warnings because the header files
 *  included below may indirectly include system header files that define
 *  prototypes for the system malloc(3) functions. The compiler gets
 *  concerned that these prototypes are extern while the ones below
 *  are static inlines.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/cxxcapi.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Heap.h"


/**
 *  Allocates a block of contiguous memory of the specified size.
 *
 *  @param  size        is the size of the requested memory
 *                      memory block in bytes.
 *
 *  @return a pointer to the suitably aligned memory block of
 *          at least the requested size, or null if an error occurred.
 */
CXXCINLINE void* desperado_malloc(size_t size) {
    return heap_malloc(platform_heap(), size);
}


/**
 *  Releases a block of memory.
 *
 *  @param  ptr         points to the memory block to release.
 *                      The pointer must have a value previously
 *                      returned by malloc(), realloc(), or calloc().
 *                      It is not an error for this pointer to be null.
 */
CXXCINLINE void desperado_free(void* ptr) {
    heap_free(platform_heap(), ptr);
}


/**
 *  Reallocate a block of contiguous memory of the specified size
 *  by allocating a new block, copying the old block into the new
 *  block, and releasing the old block (or at least giving the
 *  appearance of having done so). If the allocation of the new
 *  block fails, the old block is left untouched.
 *
 *  @param  ptr         points to the memory block to reallocate.
 *                      The pointer must have a value previously
 *                      returned by malloc() or
 *                      realloc(), or null. If it is null,
 *                      calling realloc() is equivalent
 *                      to calling malloc(size).
 *
 *  @param  size        is the size of the new requested memory
 *                      memory block in bytes. If it is zero and
 *                      ptr is not zero, calling
 *                      realloc() is equivalent to
 *                      calling free(ptr).
 *
 *  @return a pointer to the suitably aligned memory block of
 *          at least the requested size, or null if an error occurred.
 */
CXXCINLINE void* desperado_realloc(void* ptr, size_t size) {
    return heap_realloc(platform_heap(), ptr, size);
}


/**
 *  Allocate a block of contiguous memory of at least the specified
 *  number of elements of the specified size, initialized to zeros.
 *  This is useful for allocating and initializing an array.
 *
 *  @param  nmemb       is the number of members in the memory block.
 *
 *  @param  size        is the size of each member in the memory block.
 *
 *  @return a pointer to the suitably aligned and zeroed memory block of
 *          at least the requested size, or null if an error occurred.
 */
CXXCINLINE void* desperado_calloc(size_t nmemb, size_t size) {
    return heap_calloc(platform_heap(), nmemb, size);
}


#if defined(malloc)
#undef malloc
#endif
/**
 *  @def    malloc(_SIZE_)
 *
 *  Generates the code to call the Desperado malloc using the
 *  size @a _SIZE_.
 */
#define malloc(_SIZE_) desperado_malloc(_SIZE_)

#if defined(free)
#undef free
#endif
/**
 *  @def    free(_PTR_)
 *
 *  Generates the code to call the Desperado free using the
 *  pointer @a _PTR_.
 */
#define free(_PTR_) desperado_free(_PTR_)

#if defined(realloc)
#undef realloc
#endif
/**
 *  @def    realloc(_PTR_, _SIZE_)
 *
 *  Generates the code to call the Desperado realloc using the
 *  pointer @a _PTR_ and the size @a _SIZE_.
 */
#define realloc(_PTR_, _SIZE_) desperado_realloc(_PTR_, _SIZE_)

#if defined(calloc)
#undef calloc
#endif
/**
 *  @def    calloc(_NMEMB_, _SIZE_)
 *
 *  Generates the code to call the Desperado calloc using the
 *  number of members @a _NMEMB_ and the size @a _SIZE_.
 */
#define calloc(_NMEMB_, _SIZE_) desperado_calloc(_NMEMB_, _SIZE_)


#endif
