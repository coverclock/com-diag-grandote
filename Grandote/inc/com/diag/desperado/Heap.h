#ifndef _COM_DIAG_DESPERADO_HEAP_H_
#define _COM_DIAG_DESPERADO_HEAP_H_

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
 *  Declares the Heap class.
 *
 *  @see    Heap
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/cxxcapi.h"


#if defined(__cplusplus)


#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implement the interfaces to the C standard library malloc(3), free(3),
 *  realloc(3) and calloc(3) functions but using the C++ new and delete
 *  operators. This class exists to support the Ficl class, but it does offer
 *  some advantates to an application. It is implemented in terms of the
 *  C++ new and delete operators, so it allows an application to be written
 *  (o,r more typically, ported) using the old library semantics but without
 *  mixing C and C++ heap management. Since it is a class, it may be
 *  overridden, for example, in order to add instrumentation or to use as
 *  a wrapper for a different malloc library. Finally, this class has some
 *  basic instrumentation of its own that could be useful. This class mimics
 *  the semantics of the equivalent functions found in Linux 2.4.18 libc
 *  2.2.93. Of particular interest is the different behavior of the
 *  allocation functions when a size of zero is specified.
 *
 *  If included from a C translation unit, defines a C-callable API.
 *
 *  @see    malloc(3)
 *
 *  @see    free(3)
 *
 *  @see    realloc(3)
 *
 *  @see    calloc(3)
 *
 *  @see    Ficl
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Heap : public Object {

public:

    /**
     *  Constructor.
     *
     *  @param  po          points to the output object used for tracing.
     *                      If 0, the platform error object is used.
     */
    explicit Heap(Output* po = 0);

    /**
     *  Constructor.
     *
     *  @param  ro          refers to the output object used for tracing.
     */
    explicit Heap(Output& ro);

    /**
     *  Destructor.
     */
    virtual ~Heap();

    /**
     *  Returns a reference to the current heap output object.
     *
     *  @return a reference to the current output object.
     */
    virtual Output& output() const;

    /**
     *  Returns the total number of bytes allocated. The total includes
     *  any overhead introduced by this class. It does not include any
     *  overhead introduced by the underlying platform.
     *
     *  @return the total number of bytes allocated.
     */
    size_t getTotal() const;

    /**
     *  Returns the current number of bytes allocated. The total includes
     *  any overhead introduced by this class. It does not include any
     *  overhead introduced by the underlying platform.
     *
     *  @return the current number of bytes allocated.
     */
    size_t getCurrent() const;

    /**
     *  Returns the number of successful allocations.
     *
     *  @return the number of successful allocations.
     */
    size_t getSuccesses() const;

    /**
     *  Returns the number of failed allocations.
     *
     *  @return the number of failed allocations.
     */
    size_t getFailures() const;

    /**
     *  Returns the number of frees with non-null pointers.
     *
     *  @return the number of frees with non-null pointers.
     */
    size_t getFrees() const;

    /**
     *  Returns the number of frees with null pointers.
     *
     *  @return the number of frees with null pointers.
     */
    size_t getNulls() const;

    /**
     *  All memory blocks returned by the malloc(),
     *  realloc() and calloc have the memory alignment of this type.
     */
    typedef uint64_t Alignment;

    /**
     *  Allocates a block of contiguous memory of the specified size.
     *
     *  @param  size        is the size of the requested memory
     *                      memory block in bytes.
     *
     *  @return a pointer to the suitably aligned memory block of
     *          at least the requested size, or null if an error occurred.
     */
    virtual void* malloc(size_t size);

    /**
     *  Releases a block of memory.
     *
     *  @param  ptr         points to the memory block to release.
     *                      The pointer must have a value previously
     *                      returned by malloc(),
     *                      realloc(), or calloc().
     *                      It is not an error for this pointer to be null.
     */
    virtual void free(void* ptr);

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
    virtual void* realloc(void* ptr, size_t size);

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
    virtual void* calloc(size_t nmemb, size_t size);

    /**
     *  Return the size in bytes of the specified memory block.
     *
     *  @param  ptr         points to the memory block to size.
     *                      The pointer must have a value previously
     *                      returned by malloc() or
     *                      realloc().
     *
     *  @return             the size in bytes of the specified memory
     *                      block. This value is that which was specified
     *                      in the original allocation, and does not include
     *                      any overhead added by this class or the
     *                      underlying platform. Zero is returned if
     *                      a null pointer was specified.
     */
    virtual size_t size(void* ptr);

    /**
     *  Turn tracing on or off. If tracing is on, each call to a memory
     *  allocation or free method will result in at least one line of
     *  output to the output object.
     *
     *  @param  on          if true turns tracing on, otherwise tracing
     *                      is turned off.
     *
     *  @return             the prior state of tracing.
     */
    bool trace(bool on = true);

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

private:

    /**
     *  Pointer to object used for tracing.
     */
    Output* ou;

    /**
     *  Tracing on or off.
     */
    bool tracing;

    /**
     *  Total number of bytes allocated.
     */
    size_t total;

    /**
     *  Number of bytes currently allocated.
     */
    size_t current;

    /**
     *  Number of successful allocations.
     */
    size_t successes;

    /**
     *  Number of failed allocations.
     */
    size_t failures;

    /**
     *  Number of frees with non-null pointers.
     */
    size_t frees;

    /**
     *  Number of frees with null pointers.
     */
    size_t nulls;

};


//
//  Set tracing on or off.
//
inline bool Heap::trace(bool on) {
    bool was = this->tracing;
    this->tracing = on;
    return was;
}


//
//  Return the total number of bytes allocated.
//
inline size_t Heap::getTotal() const {
    return this->total;
}


//
//  Return the current number of bytes allocated.
//
inline size_t Heap::getCurrent() const {
    return this->current;
}


//
//  Return the number of successful allocations.
//
inline size_t Heap::getSuccesses() const {
    return this->successes;
}


//
//  Return the number of failed allocations.
//
inline size_t Heap::getFailures() const {
    return this->failures;
}


//
//  Return the number of frees.
//
inline size_t Heap::getFrees() const {
    return this->frees;
}

#include "com/diag/desperado/End.h"


#else


/**
 *  This is an opaque type used to refer to a heap object in a
 *  C program.
 */
typedef struct Heap Heap;


#endif


/**
 *  Allocates a block of contiguous memory of the specified size.
 *
 *  @see Heap::malloc
 *
 *  @param  h           points to a heap object.
 *
 *  @param  size        is the size of the requested memory
 *                      memory block in bytes.
 *
 *  @return a pointer to the suitably aligned memory block of
 *          at least the requested size, or null if an error occurred.
 */
CXXCAPI void* heap_malloc(CXXCTYPE(::com::diag::desperado::, Heap)* h, size_t size);


/**
 *  Releases a block of memory.
 *
 *  @see Heap::free
 *
 *  @param  h           points to a heap object.
 *
 *  @param  ptr         points to the memory block to release.
 *                      The pointer must have a value previously
 *                      returned by malloc(),
 *                      realloc(), or calloc().
 *                      It is not an error for this pointer to be null.
 */
CXXCAPI void heap_free(CXXCTYPE(::com::diag::desperado::, Heap)* h, void* ptr);


/**
 *  Reallocate a block of contiguous memory of the specified size
 *  by allocating a new block, copying the old block into the new
 *  block, and releasing the old block (or at least giving the
 *  appearance of having done so). If the allocation of the new
 *  block fails, the old block is left untouched.
 *
 *  @see Heap::realloc
 *
 *  @param  h           points to a heap object.
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
CXXCAPI void* heap_realloc(CXXCTYPE(::com::diag::desperado::, Heap)* h, void* ptr, size_t size);


/**
 *  Allocate a block of contiguous memory of at least the specified
 *  number of elements of the specified size, initialized to zeros.
 *  This is useful for allocating and initializing an array.
 *
 *  @see Heap::calloc
 *
 *  @param  h           points to a heap object.
 *
 *  @param  nmemb       is the number of members in the memory block.
 *
 *  @param  size        is the size of each member in the memory block.
 *
 *  @return a pointer to the suitably aligned and zeroed memory block of
 *          at least the requested size, or null if an error occurred.
 */
CXXCAPI void* heap_calloc(CXXCTYPE(::com::diag::desperado::, Heap)* h, size_t nmemb, size_t size);


#if defined(DESPERADO_HAS_UNITTESTS)
/**
 *  Run the Heap unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestHeap(void);
/**
 *  Run the Heap2 unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestHeap2(void);
/**
 *  Run the Heap3 unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestHeap3(void);
#endif


#endif
