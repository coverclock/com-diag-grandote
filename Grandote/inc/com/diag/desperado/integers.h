/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_DESPERADO_INTEGERS_H_
#define _COM_DIAG_DESPERADO_INTEGERS_H_

/******************************************************************************

    Copyright 2011 Digital Aggregates Corporation, Colorado, USA.
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
 *	Implements some of the more useful macros in generics.h as C++ templates.
 *	Definitely not usable anywhere but in a C++ translation unit. This is not
 *	as nearly useful as you would hope: GNU g++ 4.4.3. doesn't allow you to use
 *	these in constant expressions even though they can be resolved at compile
 *	time.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */

#include "com/diag/desperado/target.h"

#include "com/diag/desperado/Begin.h"

template <typename _TYPE_> inline size_t widthOf() {
	return sizeof(_TYPE_) * 8;
}

template <typename _TYPE_> inline size_t widthOf(_TYPE_) {
	return widthOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ signedintminOf() {
	return (static_cast<_TYPE_>(1)) << (widthOf<_TYPE_>() - 1);
}

template <typename _TYPE_> inline _TYPE_ signedintminOf(_TYPE_) {
	return signedintminOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ signedintmaxOf() {
	return ~signedintminOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ signedintmaxOf(_TYPE_) {
	return signedintmaxOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ unsignedintminOf() {
	return (static_cast<_TYPE_>(0));
}

template <typename _TYPE_> inline _TYPE_ unsignedintminOf(_TYPE_) {
	return unsignedintminOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ unsignedintmaxOf() {
	return ~unsignedintminOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ unsignedintmaxOf(_TYPE_) {
	return unsignedintmaxOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ intmaxOf() {
	return (signedintminOf<_TYPE_>() > 0) ? unsignedintmaxOf<_TYPE_>() : signedintmaxOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ intmaxOf(_TYPE_) {
	return intmaxOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ intminOf() {
	return (signedintminOf<_TYPE_>() > 0) ? unsignedintminOf<_TYPE_>() : signedintminOf<_TYPE_>();
}

template <typename _TYPE_> inline _TYPE_ intminOf(_TYPE_) {
	return intminOf<_TYPE_>();
}

#include "com/diag/desperado/End.h"


#endif
