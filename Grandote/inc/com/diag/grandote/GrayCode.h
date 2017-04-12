#ifndef _COM_DIAG_DESPERADO_GRAYCODE_H_
#define _COM_DIAG_DESPERADO_GRAYCODE_H_

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
 *  Declares the GrayCode class.
 *
 *  @see    GrayCode
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Generates methods to convert between any integral type and
 *  a binary reflected Gray code of the same width. Gray codes have the
 *  property that two successive Gray code values differ by only one
 *  bit. Specifically, G(i) and G(i+1) differ in the right-most zero
 *  bit of i. Hence, such codes are said to have a Hamming distance of
 *  one. Gray codes have lots of uses: solving the Towers of Hanoi,
 *  Hamiltonian circuits of hypercube graphs, genetic mutations,
 *  applications in unit testing, and many others. Gray codes were
 *  patented by Frank Gray of Bell Labs in 1953 where it was applied to
 *  shaft encoders used in electromechanical switchin systems. This
 *  class implements the most popular of many possible Gray codes.
 *
 *  @see    Press, W. H., <I>Numerical Recipes in C</I>,
 *          Cambridge University Press, 1992
 *
 *  @see    Gray, F., "Pulse Code Communication", U. S. patent
 *          2,632,058, March 17, 1953
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
template <typename _TYPE_>
class GrayCode : public Object {

public:

    /**
     *  Constructor.
     */
    explicit GrayCode();

    /**
     *  Destructor.
     */
    virtual ~GrayCode();

    /**
     *  This method converts an integer into its equivalent reflected
     *  Gray code.
     */
    _TYPE_ encode(_TYPE_ i) const;

    /**
     *  This method converts a reflected Gray code into its equivalent
     *  integer.
     */
    _TYPE_ decode(_TYPE_ g) const;

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

};


//
// Constructor
//
template <typename _TYPE_>
inline GrayCode<_TYPE_>::GrayCode() {
}


//
//  Destructor.
//
template <typename _TYPE_>
GrayCode<_TYPE_>::~GrayCode() {
}


//
//  Convert integer to Gray code.
//
template <typename _TYPE_>
inline _TYPE_ GrayCode<_TYPE_>::encode(_TYPE_ i) const {
    return i ^ (i >> 1);
}


//
//  Convert Gray code to integer.
//
template <typename _TYPE_>
inline _TYPE_ GrayCode<_TYPE_>::decode(_TYPE_ g) const {
    static const _TYPE_ limit = widthof(_TYPE_) / 2;
    _TYPE_ b = g;
    _TYPE_ shift = 1;
    _TYPE_ mask;

    while (true) {
        mask = b >> shift;
        b ^= mask;
        if ((1 >= mask) || (limit == shift)) {
            break;
        }
        shift <<= 1;
    }

    return b;
}


//
//  Show this object on the output object.
//
template <typename _TYPE_>
void GrayCode<_TYPE_>::show(int /* level */, Output* display, int indent) const{
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s widthof=%u\n", sp, widthof(_TYPE_));
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the GrayCode unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestGrayCode(void);
#endif


#endif
