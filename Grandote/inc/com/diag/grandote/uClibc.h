/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_DESPERADO_UCLIBC_H_
#define _COM_DIAG_DESPERADO_UCLIBC_H_

/******************************************************************************

    Copyright 2013 Digital Aggregates Corporation, Colorado, USA.
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
 *  Allows a translation unit to determine if it is running with uClibc instead
 *  of glibc.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include <sys/types.h>


#if defined(__UCLIBC_MAJOR__) && defined(__UCLIBC_MINOR__) && defined(__UCLIBC_SUBLEVEL__)
#	define DESPERADO_HAS_UCLIBC COM_DIAG_DIMINUTO_PLATFORM_UCLIBC (1000000 + ((__UCLIBC_MAJOR__) * 10000) + ((__UCLIBC_MINOR__) * 100) + (__UCLIBC_SUBLEVEL__))
#else
#	undef DESPERADO_HAS_UCLIBC
#endif


#endif
