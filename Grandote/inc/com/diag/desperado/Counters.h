#ifndef _COM_DIAG_DESPERADO_COUNTERS_H_
#define _COM_DIAG_DESPERADO_COUNTERS_H_

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
 *  Declares the Counters class.
 *
 *  @see    Counters
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Provides an interface to an array of simple counters which may be
 *  individually accessed or modified, particularly useful when all of
 *  the counters need to managed as a single object, for example
 *  when exporting counters to MIBs, or when instrumenting complex systems
 *  for the purpose of debugging during development, in situ troubleshooting,
 *  or port mortem forensics.
 *
 *  Counters may underflow or overflow. Counters are signed, and negative
 *  values are possible.
 *
 *  An array of labels may be associated with the array of counters on a
 *  one to one basis. They are displayed when the object is shown.
 *
 *  Operations past the end of array are always mapped to the last counter
 *  in the array, rather than being ignored. Hence, it is useful to
 *  allocate an extra array position at the end for the "other" counter.
 *  A user-provided label is assigned to this other counter (if labels are
 *  used at all) just as with any other counter.
 *
 *  An null-counter object can be created. Operations on such an object are
 *  benign. This allows an application to use counters optionally.
 *
 *  When using this class to support export of counters to a management
 *  information base (MIB), one approach would be for the application
 *  to offer the MIB a reference to the application's own counter object.
 *  The MIB can then reference the counters directly, mapping the current
 *  level OID to this particular counter object, and then mapping the next
 *  level OID directly to the counter number. The application could even
 *  assign the actual MIB variable names to the counter labels.
 *
 *  Because counters may be used in device drivers, including even interrupt
 *  service routines, it is important that the overhead of using counters
 *  is a small as possible.
 *
 *  @see    L. Rising, ed., <I>Design Patterns in Communications Systems</I>,
 *          Cambridge University Press, 2001
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Counters : public Object {

public:

    /**
     *  This is the type of an individual counter variable.
     */
    typedef int Counter;

    /**
     *  If the class generates a label for a counter, this is the type
     *  of the buffer into which the characters are placed.
     */
    typedef char Label[sizeof("N0000000000")];

    /**
     *  Constructor.
     *
     *  @param  ncounters           is the number of counters in the
     *                              array of counters provided by the
     *                              caller. This parameter may be zero,
     *                              in which case the operations performed
     *                              on the object are benign.
     *
     *  @param  vcounters           is an array of counters provided by
     *                              the caller. This array is not modified
     *                              during ocnstruction, and is assumed to
     *                              be pre-initialized. If the number of
     *                              counters is zero, this parameter may be
     *                              null.
     *
     *  @param  vlabels             is an array of labels that correspond
     *                              one-to-one to the counters, for when
     *                              the counters are shown. The array may
     *                              be omitted by passing null, in which case
     *                              labels are not used.
     */
    explicit Counters(
        size_t ncounters = 0,
        Counter* vcounters = 0,
        const char** vlabels = 0
    );

    /**
     *  Destructor.
     */
    virtual ~Counters();

    /**
     *  Reset all counters in this object to a value.
     *
     *  @param  value               is the value to which the counters
     *                              are set. If no value is specified,
     *                              zero is used.
     */
    virtual void reset(Counter value = 0);

    /**
     *  Defines an interface to a functor (an object that can
     *  be called like a function), that performs an operation on an
     *  individual counter.
     */
    class Functor {

    public:

        /**
         *  Dtor.
         */
        virtual ~Functor();

        /**
         *  Perform an application-defined operation on an individual
         *  counter.
         *
         *  @param  id          is the counter identifier.
         *
         *  @param  label       is the label of the individual counter.
         *                      If there are no labels, a printable label is
         *                      generated based on the counter identifer.
         *
         *  @param  value       is the counter value.
         *
         *  @return true if the operation was successful, false
         *          otherwise.
         */
        virtual bool operator() (
            unsigned int id,
            const char* label,
            Counter value
        ) = 0;

    };

    /**
     *  Applies a functor once and only once to each counter in this
     *  object, until the functor returns false, or until the counters
     *  are exhausted.
     *
     *  @param  functor         refers to the functor to be applied.
     *
     *  @return true if the functor returned true for all applications,
     *          false as soon as the functor returns false.
     */
    bool apply(Functor& functor);

    /**
     *  Makes a label for the specified counter.
     *
     *  @param  id              is the counter identifier.
     *
     *  @param  buffer          is where the label is built, if necessary.
     *
     *  @return a pointer to the label.
     */
    const char* label(unsigned int id, Label buffer) const;

    /**
     *  Gets the value of the specified counter.
     *
     *  @param  id              is the counter identifier.
     *
     *  @return the value of the specified counter.
     */
    Counter get(unsigned int id) const;

    /**
     *  Sets the specified counter to the indicated value.
     *  The value may be negative. By default it is zero.
     *
     *  @param  id              is the counter identifier.
     *
     *  @param  value           is the value to which the counter is set.
     *
     *  @return the prior counter value.
     */
    Counter set(unsigned int id, Counter value = 0);

    /**
     *  Increments the specified counter by the indicated value.
     *  The value may be negative. By default it is one.
     *
     *  @param  id              is the counter identifier.
     *
     *  @param  value           is the value by which the counter is changed.
     *
     *  @return the prior counter value.
     */
    Counter increment(unsigned int id, Counter value = 1);

    /**
     *  Decrements the specified counter by the indicated value.
     *  The value may be negative. By default it is one.
     *
     *  @param  id              is the counter identifier.
     *
     *  @param  value           is the value by which the counter is changed.
     *
     *  @return the prior counter value.
     */
    Counter decrement(unsigned int id, Counter value = 1);

    /**
     *  Returns the number of counters stored in this object.
     *
     *  @return the number of counters in this object.
     */
    size_t cardinal() const;

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
     *  Generates a valid counter array index given a counter number
     *  which may or may not be within range.
     *
     *  @param  id              is the counter identifier.
     *
     *  @return a valid counter array index.
     */
    unsigned int validate(unsigned int id) const;

    /**
     *  Generate a label for the counter at the given array index.
     *
     *  @param  index           is the counter's position in the array.
     *
     *  @param  buffer          is where the label is built, if necessary.
     *
     *  @return a string identifying the counter.
     */
    const char* generate(unsigned int index, Label buffer) const;

    /**
     *  Generate a counter representation for the counter at the given
     *  array index.
     *
     *  @param  index           is the counter's position in the array.
     *
     *  @param  buffer          is where the representation is built.
     *
     *  @return a string representing the counter.
     */
    const char* format(unsigned int index, Label buffer) const;

    /**
     *  Number of entries in allocated array of counters.
     */
    size_t countersn;

    /**
     *  Pointer to an array of integer counters.
     */
    Counter* countersv;

private:

    /**
     *  Pointer to an array of counter labels.
     */
    const char** labelsv;

};


//
//  Return a valid counter array index given a counter identifier.
//
inline unsigned int Counters::validate(unsigned int id) const {
    return (this->countersn > id) ? id : (this->countersn - 1);
}


//
//  Get a counter.
//
inline Counters::Counter Counters::get(unsigned int id) const {
    return (0 != this->countersv) ? this->countersv[this->validate(id)] : 0;
}


//
//  Set a counter.
//
inline Counters::Counter Counters::set(unsigned int id, Counter value) {
    Counter prior = 0;
    if (0 < this->countersn) {
        unsigned int index = this->validate(id);
        prior = this->countersv[index];
        this->countersv[index] = value;
    }
    return prior;
}


//
//  Increment a counter.
//
inline Counters::Counter Counters::increment(unsigned int id, Counter value) {
    Counter prior = 0;
    if (0 < this->countersn) {
        unsigned int index = this->validate(id);
        prior = this->countersv[index];
        this->countersv[index] += value;
    }
    return prior;
}


//
//  Decrement a counter.
//
inline Counters::Counter Counters::decrement(unsigned int id, Counter value) {
    return this->increment(id, -value);
}



//
//  Return the number of counters in this object.
//
inline size_t Counters::cardinal() const {
    return this->countersn;
}


//
//  Return a label for a counter.
//
inline const char* Counters::label(unsigned int id, Label buffer) const {
    return (0 < this->countersn)
        ? this->generate(this->validate(id), buffer)
        : "";
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Counters unit test.
 *
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestCounters(void);
#endif


#endif
