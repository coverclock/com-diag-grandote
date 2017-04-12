#ifndef _COM_DIAG_DESPERADO_VINTAGE_H_
#define _COM_DIAG_DESPERADO_VINTAGE_H_

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
 *  Declares the Vintage class.
 *
 *  @see    Vintage
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Contains static variables with control information about
 *  this product. The base class contains information about
 *  the Desperado library itself. A derived class may
 *  contain information about an application using the library.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Vintage : public Object {

public:

    /**
     *  This NUL-terminated string contains the product name.
     */
    static const char product[];

    /**
     *  This NUL-terminated string contains the license terms.
     */
    static const char license[];

    /**
     *  This NUL-terminated string contains the copyright notice.
     */
    static const char copyright[];

    /**
     *  This NUL-terminated string contains the contact information.
     */
    static const char contact[];

    /**
     *  This NUL-terminated string contains the URL information.
     */
    static const char url[];

    /**
     *  This NUL-terminated string contains the release name.
     */
    static const char release[];

    /**
     *  This NUL-terminated string contains the version number.
     */
    static const char version[];

    /**
     *  This NUL-terminated string contains the hardware target.
     */
    static const char target[];

    /**
     *  This NUL-terminated string contains the operating system platform.
     */
    static const char platform[];

    /**
     *  This NUL-terminated string contains the release date.
     */
    static const char date[];

    /**
     *  This NUL-terminated string contains the build date and time.
     */
    static const char build[];

    /**
     *  Constructor.
     */
    explicit Vintage();

    /**
     *  Destructor.
     */
    virtual ~Vintage();

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
inline Vintage::Vintage() {
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Vintage unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestVintage(void);
#endif


#endif
