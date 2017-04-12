/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_DESPERADO_READY_H_
#define _COM_DIAG_DESPERADO_READY_H_

/******************************************************************************

    Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA.
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
 *  Defines the Desperado select-based I/O ready indication.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/cxxcapi.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/uClibc.h"


#define DESPERADO_DESCRIPTOR_READY_READ			(1<<0)
#define DESPERADO_DESCRIPTOR_READY_WRITE		(1<<1)
#define DESPERADO_DESCRIPTOR_READY_EXCEPTION	(1<<2)
#define DESPERADO_DESCRIPTOR_READY_ERROR		(1<<(widthof(int)-1))


/**
 * Provides an indication whether the specified descriptor is ready for read,
 * write, or exception. (Exception is not well defined.) This function is non-
 * blocking. If the underlying system call fails, 0 is returned.
 * @param fd is the file descriptor.
 * @return 0 for not ready, masked with defined bits if ready, <0 if error.
 */
CXXCAPI int desperado_descriptor_ready(int fd);

/**
 * Returns the number of bytes readable from the standard I/O buffer for the
 * specified FILE pointer.
 * THIS FUNCTION IS NOT THREAD SAFE.
 * It's not especially portable either.
 * @param fp points to the FILE structure.
 * @return the number of readable bytes.
 */
CXXCINLINE size_t desperado_file_readable(FILE * fp) {
#if !defined(DESPERADO_HAS_UCLIBC)
	return (fp != 0) ? ((fp->_IO_read_ptr < fp->_IO_read_end) ? fp->_IO_read_end - fp->_IO_read_ptr : 0) : 0;
#elif defined(__STDIO_BUFFERS) && defined(__STDIO_GETC_MACRO)
	return (fp != 0) ? ((fp->__bufpos < fp->__bufgetc_u) ? fp->__bufgetc_u - fp->__bufpos : 0) : 0;
#else
#	warning desperado_file_readable stubbed!
	return -1;
#endif
}

/**
 * Returns the number of bytes writeable from the standard I/O buffer for the
 * specified FILE pointer.
 * THIS FUNCTION IS NOT THREAD SAFE.
 * It's not especially portable either.
 * @param fp points to the FILE structure.
 * @return the number of writeable bytes.
 */
CXXCINLINE size_t desperado_file_writeable(FILE * fp) {
#if !defined(DESPERADO_HAS_UCLIBC)
	return (fp != 0) ? ((fp->_IO_write_ptr < fp->_IO_write_end) ? fp->_IO_write_end - fp->_IO_write_ptr : 0) : 0;
#elif defined(__STDIO_BUFFERS) && defined(__STDIO_PUTC_MACRO)
	return (fp != 0) ? ((fp->__bufpos < fp->__bufputc_u) ? fp->__bufputc_u - fp->__bufpos : 0) : 0;
#else
#	warning desperado_file_writeable stubbed!
	return -1;
#endif
}


#endif
