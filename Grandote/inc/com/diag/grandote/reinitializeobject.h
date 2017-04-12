#ifndef _COM_DIAG_DESPERADO_REINITIALIZEOBJECT_H_
#define _COM_DIAG_DESPERADO_REINITIALIZEOBJECT_H_

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
 *  Defines the terrifying reinitializeobject preprocessor macro. The mere
 *  definition of this and related macros, never mind its actual use, borders
 *  on the criminally stupid. However, if you really want to understand how
 *  C++ works under the hood, debugging applications that uses this macro will
 *  certainly help with that.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include <new>


/**
 *  @def    reinitializeobject(_CLASS_, _REFERENCE_)
 *
 *  Re-initializes the object of type @a _CLASS_ referred to by
 *  the reference @a _REFERENCE_ by directly invoking its destructor
 *  followed by its constructor with the placement new operator.
 *
 *  Using this macro is fraught with peril. The technique is so clever,
 *  it borders on criminally stupid. This macro is very useful for
 *  reinitializing an object by exploiting the class' destructor
 *  and constructors, without relying on user-defined settor or
 *  initialization methods. But calling it against a derived class
 *  object using a base class type and pointer results in its vtable
 *  being reinitialized with the base class method addresses. This
 *  causes the derived class object to exhibit base class behaviors.
 *
 *  However, have said all of that, this macro is remarkably useful
 *  if used carefully. See the associated unit test for a demonstration.
 *
 *  N.B.    I just spent four hours squashing a bug I introduced because
 *          used this macro. It has got to go.
 * 
 * @see reinitializeobjectwith
 */
#define reinitializeobject(_CLASS_, _REFERENCE_) \
    do { \
        (_REFERENCE_)._CLASS_::~_CLASS_(); \
        new(&(_REFERENCE_)) _CLASS_; \
    } while (false)


/**
 *  @def    reinitializeobjectwith(_CLASS_, _REFERENCE_, _CTORARGS_)
 *
 *  Re-initializes the object of type @a _CLASS_ referred to by
 *  the reference @a _REFERENCE_ by directly invoking its destructor
 *  followed by its constructor with the argument list @a _CTORARGS_
 *  with the placement new operator.
 * 
 *  This macro is as equally malevolent as reinitializeobject.
 *
 *  @see    reinitializeobject
 */
#define reinitializeobjectwith(_CLASS_, _REFERENCE_, _CTORARGS_) \
    do { \
        (_REFERENCE_)._CLASS_::~_CLASS_(); \
        new(&(_REFERENCE_)) _CLASS_ _CTORARGS_; \
    } while (false)


#endif
