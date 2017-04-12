#ifndef _COM_DIAG_DESPERADO_READS_H_
#define _COM_DIAG_DESPERADO_READS_H_

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
 *  Declares the reads function. This can be included
 *  from either a C or a C++ translation unit.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/cxxcapi.h"


/**
 *  Repeatedly performs a data read against the specified file
 *  descriptor until the specified number of bytes have been read
 *  or until EOF or an error occurs.
 *
 *  @see    writes
 *
 *  @param  fd          refers to the file descriptor.
 *
 *  @param  pointer     points to the data buffer.
 *
 *  @param  atleast     is the minimum number of bytes to read.
 *
 *  @param  nomore      is the maximum number of bytes to read.
 *
 *  @return the requested number of bytes if all were successfully
 *          read, the actual number of bytes read, which may be zero
 *          if EOF was encountered, or the negative of one more than
 *          the number of actual number of bytes read if an error
 *          occurred.
 */
CXXCAPI ssize_t desperado_reads(
    int fd,
    void* pointer,
    size_t atleast,
    size_t nomore
);


#endif
