#ifndef _COM_DIAG_DESPERADO_IMPLEMENTATION_H_
#define _COM_DIAG_DESPERADO_IMPLEMENTATION_H_

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
 *  Declares the Implementation class.
 *
 *  @see    Implementation
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Abstract.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  This is a pilot framework for a header file declaring an implementation
 *  class. It is used to test new designs.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Implementation : public Abstract {

public:

    /**
     *  Constructor.
     *
     *  Declaring constructors explicit will prevent the compiler from
     *  using them unexpectedly via promotion of arguments. A constructor
     *  with no arguments, such as this one, carries no such risk, but
     *  is declared explicit anyway as a general policy. This is kind
     *  of a pain, since arguments may have to be explicitly cast to
     *  the correct types (in particular, const), but it makes it less
     *  likely that the compiler will create temporaries without the
     *  approval of the developer.
     *
     *  Making this constructor protected can be used to insure that
     *  an object of this class cannot be constructed by itself, but
     *  only in the context of constructing a derived class object.
     *  However, this also prevents the use of placement-new on the
     *  base class portion of the derived class object.
     *
     *  Providing a zero-argument constructor facilitates, among other
     *  things, the ability to have pools of objects of this type, each
     *  instance of which are re-constructed using placement-new when
     *  they are allocated from the pool and placed into service.
     *
     *  Consider whether this constructor can be called as part of
     *  static initialization before main is called, and perhaps before
     *  any other classes have initialized. To facilitate this capability,
     *  it is useful for the constructor not to reference any other
     *  classes (including Platform) which may not have yet been initialized.
     *  This may require lazy initialization of the object of this type,
     *  in which it is not fully constructed until the first time it is
     *  used. This further complicates multithreaded applications, since
     *  the lazy initialization may need to be in a critical section.
     */
    explicit Implementation();

    /**
     *  Constructor.
     *
     *  @param  f       initializes the field.
     */
    explicit Implementation(void* f);

    /**
     *  Destructor.
     *
     *  Unless there is a compelling reason not, to all destructors should
     *  be virtual so that derived objects may be safely pointed or referred
     *  to by base class pointers or references. An example of a compelling
     *  reason is a need to minimize its memory footprint that is so
     *  critical that even the single virtual table pointer in the object
     *  is an issue, in which case none of the methods in this class or in
     *  any base class may be virtual.
     *
     *  The destructor must be smart enough to be called multiple times
     *  on the same object and know what needs to be done, if anything.
     */
    virtual ~Implementation();

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
     *  Copy constructor.
     *
     *  Making this constructor private and leaving it unimplemented will
     *  eliminate the capability for objects of this type to be constructed
     *  by copying from another object of this type.
     *
     *  Making it public and implementing it will allow objects of this
     *  type to be copied, and allows the implementation of any special
     *  deep copy needs.
     *
     *  Making it protected and implementing it will allow only derived class
     *  objects to be copied if they are implemented to permit it.
     *
     *  Omitting it completely allows objects of this type to be copied
     *  using the synthesized copy constructor.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Implementation(const Implementation& that);

    /**
     *  Assignment operator.
     *
     *  Making this operator private and leaving it unimplemented will
     *  eliminate the capability for objects of this type to be assigned
     *  by copying from another object of this type.
     *
     *  Making it public and implementing it will allow objects of this
     *  type to be assigned to, and allows the implementation of any special
     *  deep copy needs.
     *
     *  Making it protected and implementing it will allow only derived class
     *  objects to be assigned to if they are implemented to permit it.
     *
     *  Omitting it completely allows objects of this type to be assign
     *  to using the synthesized assignment operator.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Implementation& operator=(const Implementation& that);

private:

    /**
     *  This is an example field. As a general policy, make fields
     *  private, unless there is a compelling reason not to. Accessor
     *  and settor methods can be made protected if there is a need.
     */
    void* field;

};

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Implementation unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestImplementation(void);
#endif


#endif
