//* vim: set ts=4 expandtab shiftwidth=4: */

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
 *  Implements the Desperado select-based I/O ready indication.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#if !0
#include <sys/select.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <cstring>
#include "com/diag/desperado/ready.h"


CXXCAPI int desperado_descriptor_ready(int fd) {
	int result = 0;

	fd_set readable;
	FD_ZERO(&readable);
	FD_SET(fd, &readable);

	fd_set writeable;
	FD_ZERO(&writeable);
	FD_SET(fd, &writeable);

	fd_set exceptional;
	FD_ZERO(&exceptional);
	FD_SET(fd, &exceptional);

	struct timeval timeout;
	std::memset(&timeout, 0, sizeof(timeout));

	int rc = ::select(fd + 1, &readable, &writeable, &exceptional, &timeout);
	if (0 < rc) {
		if (FD_ISSET(fd, &readable)) {
			result |= DESPERADO_DESCRIPTOR_READY_READ;
		}
		if (FD_ISSET(fd, &writeable)) {
			result |= DESPERADO_DESCRIPTOR_READY_WRITE;
		}
		if (FD_ISSET(fd, &exceptional)) {
			result |= DESPERADO_DESCRIPTOR_READY_EXCEPTION;
		}
	} else if (0 > rc) {
		result |= DESPERADO_DESCRIPTOR_READY_ERROR;
	} else {
		// Do nothing.
	}

	return result;
}
