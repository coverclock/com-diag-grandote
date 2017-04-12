#ifndef _COM_DIAG_DESPERADO_FIFO_H_
#define _COM_DIAG_DESPERADO_FIFO_H_

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
 *  Declares the Fifo class.
 *
 *  @see    Fifo
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Generates a very lightweight mechanism for implementing a circular
 *  buffer for objects of the specified type, most usefully in
 *  device drivers, including their interrupt service routines.
 *  The type of object queued is defined by the application. It could
 *  be characters, words, blocks, samples, or device register contents.
 *  Objects are queued by value, that is, by copying them into the queue.
 *  The memory for the queue is provided by the application. The queue
 *  size, measured in objects, is limited to a power of two. For example,
 *  if a queue of one hundred bytes is provided to store objects that are
 *  four-byte words, only sixty-four bytes (sixteen words) will be used.
 *
 *  No implicit synchronization or critical section is implemented. This
 *  is the responsibility of the application. The value returned by
 *  used() plus the number returned by free()
 *  may not equal the number returned by total() in any
 *  system in which multiple threads have access to this object.
 *
 *  Only the destructor and the show method are virtual. All other methods
 *  are inline.
 *
 *  @see    J. Ganssle, "A Fiendishly Clever Circular Buffer",
 *          <I>The Embedded Muse</I>, issue 110, February 17 2005
 *
 *  @author coverclock@diag.com (Chip Overclock),
 *          Phil Ouelette,
 *          Jack Ganssle
 */
template <typename _TYPE_>
class Fifo : public Object {

public:

    /**
     *  Constructor.
     *
     *  @param  qq          points the the start of an array of objects
     *                      used as the queue.
     *
     *  @param  cc          is the countof of the number of objects in
     *                      the array used as the queue.
     */
    explicit Fifo(_TYPE_* qq = 0, size_t cc = 0);

    /**
     *  Destructor.
     */
    virtual ~Fifo();

    /**
     *  Returns the number of used entries in the queue.
     *
     *  @return the number of used entries.
     */
    size_t used() const;

    /**
     *  Returns the number of free entries in the queue.
     *
     *  @return the number of free entries.
     */
    size_t free() const;

    /**
     *  Returns the total number of entries in the queue.
     *
     *  @return the number of free entries.
     */
    size_t total() const;

    /**
     *  Returns the queue to its empty state.
     */
    void reset();

    /**
     *  Inserts an object into the queue. The object is copied into
     *  an empty position in the queue. The object type must permit
     *  assignment semantics.
     *
     *  @param  entry   refers to the object from which a copy is
     *                  made into the next unused object in the queue.
     *
     *  @return true if successful, false if the queue is full.
     */
    bool insert(const _TYPE_& entry);

    /**
     *  Makes a copy of the first item on the queue, but does not
     *  remove it from the queue.
     *
     *  @param  result  refers to the object into which the next used
     *                  object is copied.
     *
     *  @return true if successful, false if the queue is empty.
     */
    bool peek(_TYPE_& result) const;

    /**
     *  Makes a copy of the first item on the queue, and removes it
     *  from the queue.
     *
     *  @param  result  refers to the object into which the next used
     *                  object is copied.
     *
     *  @return true if successful, false if the queue is empty.
     */
    bool remove(_TYPE_& result);

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
     *  This refers to the queue array provided by the application.
     */
    _TYPE_* queue;

private:

    /**
     *  This is the countof the number of objects that can be in the queue.
     */
    size_t count;

    /**
     *  This is one less than the number of possible objects in the queue.
     */
    size_t mask;

    //  It is a matter of personal preference as to whether you insert at
    //  the head and remove from the tail, as Ouellette's original algorithm
    //  implemented here does, or vice versa. I typically do the reverse.

    /**
     *  This indexes the next unused object in the queue.
     */
    size_t head;

    /**
     *  This indexes the next used object in the queue.
     */
    size_t tail;

};


//
// Constructor
//
template <typename _TYPE_>
Fifo<_TYPE_>::Fifo(_TYPE_* qq, size_t cc) :
    queue(qq),
    count(0),
    mask(0),
    head(0),
    tail(0)
{
    if (0 != qq) {
        for (size_t bit = 1; ((0 < bit) && (bit <= cc)); bit <<= 1) {
            this->count = bit;
        }
        if (0 < this->count) {
            this->mask = this->count - 1;
        }
    }
}


//
//  Destructor.
//
template <typename _TYPE_>
Fifo<_TYPE_>::~Fifo() {
}


//
//  Reset to empty state.
//
template <typename _TYPE_>
inline void Fifo<_TYPE_>::reset() {
    this->head = 0;
    this->tail = 0;
}


//
//  Return the number of used entries. No, this really is correct as
//  written. Ganssle is right, this algorithm is almost too clever
//  for its own good.
//
template <typename _TYPE_>
inline size_t Fifo<_TYPE_>::used() const {
    return this->head - this->tail;
}


//
//  Return the total number of entries.
//
template <typename _TYPE_>
inline size_t Fifo<_TYPE_>::total() const {
    return this->count;
}


//
//  Return the number of free entries.
//
template <typename _TYPE_>
inline size_t Fifo<_TYPE_>::free() const {
    return this->count - (this->head - this->tail);
}


//
//  Insert new entry into queue.
//
template <typename _TYPE_>
bool Fifo<_TYPE_>::insert(const _TYPE_& entry) {
    if (this->free() > 0) {
        this->queue[(this->head++) & this->mask] = entry;
        return true;
    } else {
        return false;
    }
}


//
//  Peek at oldest used entry in queue.
//
template <typename _TYPE_>
bool Fifo<_TYPE_>::peek(_TYPE_& result) const {
    if (this->used() > 0) {
        result = this->queue[this->tail & this->mask];
        return true;
    } else {
        return false;
    }
}


//
//  Remove oldest used entry in queue.
//
template <typename _TYPE_>
bool Fifo<_TYPE_>::remove(_TYPE_& result) {
    if (this->used() > 0) {
        result = this->queue[(this->tail++) & this->mask];
        return true;
    } else {
        return false;
    }
}


//
//  Show this object on the output object.
//
template <typename _TYPE_>
void Fifo<_TYPE_>::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s widthof=%u\n", sp, widthof(_TYPE_));
    printf("%s count=%u\n", sp, this->count);
    printf("%s mask=0x%x\n", sp, this->mask);
    size_t ii = this->head & this->mask;
    printf("%s head=%u:%u:%p\n",
        sp, this->head, ii, &(this->queue[ii]));
    ii = this->tail & this->mask;
    printf("%s tail=%u:%u:%p\n",
        sp, this->tail, ii, &(this->queue[ii]));
    printf("%s used()=%u\n", sp, this->used());
    printf("%s free()=%u\n", sp, this->free());
    printf("%s total()=%u\n", sp, this->total());
    if ((0 == level) || (0 == this->mask)) {
        printf("%s queue=%p\n", sp, this->queue);
    } else  {
        printf("%s queue:\n", sp);
        Dump dump;
        dump.bytes(this->queue, this->count * sizeof(_TYPE_), false, 0,
            indent + 2);
    }
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Fifo unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestFifo(void);
#endif


#endif
