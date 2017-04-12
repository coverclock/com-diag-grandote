#ifndef _COM_DIAG_DESPERADO_MINIMUM_H_
#define _COM_DIAG_DESPERADO_MINIMUM_H_

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
 *  Declares the Minimum class.
 *
 *  @see    Minimum
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Begin.h"

/**
 *  Generates a class containing functors to find the maximum from a
 *  list of objects.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
template <typename _TYPE_>
class Minimum {

public:

    /**
     *  Return true of the first object is less than the
     *  second object.
     *
     *  @param  one         is the first object.
     *
     *  @param  two         is the second object.
     *
     *  @return             true if one is less than two.
     */
    bool less(
        _TYPE_ one,
        _TYPE_ two
    );

    /**
     *  Return the minimum object in an argument list of
     *  at one objects.
     *
     *  @param  one         is the first object in the
     *                      argument list.
     *
     *  @return             the minimum object in the argument
     *                      list.
     */
    _TYPE_ operator() (
        _TYPE_ one
    );

    /**
     *  Return the minimum object in an argument list of
     *  at two objects.
     *
     *  @param  one         is the first object in the
     *                      argument list.
     *
     *  @param  two         is the second object in the
     *                      argument list.
     *
     *  @return             the minimum object in the argument
     *                      list.
     */
    _TYPE_ operator() (
        _TYPE_ one,
        _TYPE_ two
    );

    /**
     *  Return the minimum object in an argument list of
     *  at two objects.
     *
     *  @param  one         is the first object in the
     *                      argument list.
     *
     *  @param  two         is the second object in the
     *                      argument list.
     *
     *  @param  three       is the third object in the
     *                      argument list.
     *
     *  @return             the minimum object in the argument
     *                      list.
     */
    _TYPE_ operator() (
        _TYPE_ one,
        _TYPE_ two,
        _TYPE_ three
    );

    /**
     *  Return the minimum object in an argument list of
     *  at two objects.
     *
     *  @param  one         is the first object in the
     *                      argument list.
     *
     *  @param  two         is the second object in the
     *                      argument list.
     *
     *  @param  three       is the third object in the
     *                      argument list.
     *
     *  @param  four        is the fourth object in the
     *                      argument list.
     *
     *  @return             the minimum object in the argument
     *                      list.
     */
    _TYPE_ operator() (
        _TYPE_ one,
        _TYPE_ two,
        _TYPE_ three,
        _TYPE_ four
    );

    /**
     *  Return the minimum object in an argument list of
     *  at two objects.
     *
     *  @param  one         is the first object in the
     *                      argument list.
     *
     *  @param  two         is the second object in the
     *                      argument list.
     *
     *  @param  three       is the third object in the
     *                      argument list.
     *
     *  @param  four        is the fourth object in the
     *                      argument list.
     *
     *  @param  five        is the fifth object in the
     *                      argument list.
     *
     *  @return             the minimum object in the argument
     *                      list.
     */
    _TYPE_ operator() (
        _TYPE_ one,
        _TYPE_ two,
        _TYPE_ three,
        _TYPE_ four,
        _TYPE_ five
    );

    /**
     *  Return the minimum object in an argument list of
     *  at two objects.
     *
     *  @param  one         is the first object in the
     *                      argument list.
     *
     *  @param  two         is the second object in the
     *                      argument list.
     *
     *  @param  three       is the third object in the
     *                      argument list.
     *
     *  @param  four        is the fourth object in the
     *                      argument list.
     *
     *  @param  five        is the fifth object in the
     *                      argument list.
     *
     *  @param  six         is the sixth object in the
     *                      argument list.
     *
     *  @return             the minimum object in the argument
     *                      list.
     */
    _TYPE_ operator() (
        _TYPE_ one,
        _TYPE_ two,
        _TYPE_ three,
        _TYPE_ four,
        _TYPE_ five,
        _TYPE_ six
    );

    /**
     *  Return the minimum object in an argument list of
     *  at two objects.
     *
     *  @param  one         is the first object in the
     *                      argument list.
     *
     *  @param  two         is the second object in the
     *                      argument list.
     *
     *  @param  three       is the third object in the
     *                      argument list.
     *
     *  @param  four        is the fourth object in the
     *                      argument list.
     *
     *  @param  five        is the fifth object in the
     *                      argument list.
     *
     *  @param  six         is the sixth object in the
     *                      argument list.
     *
     *  @param  seven       is the seventh object in the
     *                      argument list.
     *
     *  @return             the minimum object in the argument
     *                      list.
     */
    _TYPE_ operator() (
        _TYPE_ one,
        _TYPE_ two,
        _TYPE_ three,
        _TYPE_ four,
        _TYPE_ five,
        _TYPE_ six,
        _TYPE_ seven
    );

    /**
     *  Return the minimum object in an argument list of
     *  at two objects.
     *
     *  @param  one         is the first object in the
     *                      argument list.
     *
     *  @param  two         is the second object in the
     *                      argument list.
     *
     *  @param  three       is the third object in the
     *                      argument list.
     *
     *  @param  four        is the fourth object in the
     *                      argument list.
     *
     *  @param  five        is the fifth object in the
     *                      argument list.
     *
     *  @param  six         is the sixth object in the
     *                      argument list.
     *
     *  @param  seven       is the seventh object in the
     *                      argument list.
     *
     *  @param  eight       is the eighth object in the
     *                      argument list.
     *
     *  @return             the minimum object in the argument
     *                      list.
     */
    _TYPE_ operator() (
        _TYPE_ one,
        _TYPE_ two,
        _TYPE_ three,
        _TYPE_ four,
        _TYPE_ five,
        _TYPE_ six,
        _TYPE_ seven,
        _TYPE_ eight
    );

};


template <typename _TYPE_>
inline bool Minimum<_TYPE_>::less(
    _TYPE_ one,
    _TYPE_ two
) {
    return (one < two);
}


template <typename _TYPE_>
inline _TYPE_ Minimum<_TYPE_>::operator() (
    _TYPE_ one
) {
    return one;
}


template <typename _TYPE_>
inline _TYPE_ Minimum<_TYPE_>::operator() (
    _TYPE_ one,
    _TYPE_ two
) {
    return this->less(one, two) ? one : two;
}


template <typename _TYPE_>
inline _TYPE_ Minimum<_TYPE_>::operator() (
    _TYPE_ one,
    _TYPE_ two,
    _TYPE_ three
) {
    return (*this)((*this)(one, two), three);
}


template <typename _TYPE_>
inline _TYPE_ Minimum<_TYPE_>::operator() (
    _TYPE_ one,
    _TYPE_ two,
    _TYPE_ three,
    _TYPE_ four
) {
    return (*this)((*this)(one, two, three), four);
}


template <typename _TYPE_>
inline _TYPE_ Minimum<_TYPE_>::operator() (
    _TYPE_ one,
    _TYPE_ two,
    _TYPE_ three,
    _TYPE_ four,
    _TYPE_ five
) {
    return (*this)((*this)(one, two, three, four), five);
}


template <typename _TYPE_>
inline _TYPE_ Minimum<_TYPE_>::operator() (
    _TYPE_ one,
    _TYPE_ two,
    _TYPE_ three,
    _TYPE_ four,
    _TYPE_ five,
    _TYPE_ six
) {
    return (*this)((*this)(one, two, three, four, five), six);
}


template <typename _TYPE_>
inline _TYPE_ Minimum<_TYPE_>::operator() (
    _TYPE_ one,
    _TYPE_ two,
    _TYPE_ three,
    _TYPE_ four,
    _TYPE_ five,
    _TYPE_ six,
    _TYPE_ seven
) {
    return (*this)((*this)(one, two, three, four, five, six), seven);
}


template <typename _TYPE_>
inline _TYPE_ Minimum<_TYPE_>::operator() (
    _TYPE_ one,
    _TYPE_ two,
    _TYPE_ three,
    _TYPE_ four,
    _TYPE_ five,
    _TYPE_ six,
    _TYPE_ seven,
    _TYPE_ eight
) {
    return (*this)((*this)(one, two, three, four, five, six, seven), eight);
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Minimum and Maximum unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestMinimumMaximum(void);
#endif


#endif
