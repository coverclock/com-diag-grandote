/* vim: set ts=4 expandtab shiftwidth=4: */

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
 *  Implements the reads utility.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include <unistd.h>
#include "com/diag/desperado/reads.h"


CXXCAPI ssize_t desperado_reads(
    int fd,
    void* pointer,
    size_t atleast,
    size_t nomore
) {
    char* here;
    size_t remaining;
    ssize_t total;
    ssize_t rc;

    here = static_cast<char*>(pointer);
    remaining = nomore;
    total = 0;

    while (static_cast<size_t>(total) < atleast) {
        rc = ::read(fd, here, remaining);
        if (0 == rc) {
            break;
        }
        if (0 > rc) {
            total = -(total + 1);
            break;
        }
        here += rc;
        remaining -= rc;
        total += rc;
    }

    return total;
}
