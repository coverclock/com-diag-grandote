/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the Platform C API.
 *
 *  @see    Platform
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Desperado.h"


CXXCAPI ticks_t platform_frequency() {
    return Platform::instance().frequency();
}


CXXCAPI ticks_t platform_time() {
    return Platform::instance().time();
}


CXXCAPI ticks_t platform_elapsed() {
    return Platform::instance().elapsed();
}


CXXCAPI ticks_t platform_yield(ticks_t ticks, int premature) {
    return Platform::instance().yield(ticks, (premature != 0));
}


CXXCAPI identity_t platform_identity() {
    return Platform::instance().identity();
}


CXXCAPI void platform_fatal(
    const char* event,
    int error,
    const char* file,
    int line,
    const char* function
) {
    Platform::instance().fatal(event, error, file, line, function);
}


CXXCAPI Input* platform_input() {
    return &(Platform::instance().input());
}


CXXCAPI Output* platform_output() {
    return &(Platform::instance().output());
}


CXXCAPI Output* platform_error() {
    return &(Platform::instance().error());
}


CXXCAPI Dump* platform_dump() {
    return &(Platform::instance().dump());
}


CXXCAPI Heap* platform_heap() {
    return &(Platform::instance().heap());
}


CXXCAPI Logger* platform_logger() {
    return &(Platform::instance().logger());
}


CXXCAPI const char* platform_platform() {
    return Platform::instance().platform();
}


CXXCAPI const char* platform_target() {
    return Platform::instance().target();
}


CXXCAPI const char* platform_host() {
    return Platform::instance().host();
}


CXXCAPI ssize_t platform_printf(const char* format, ...) {
    va_list ap;
    ssize_t rc;

    va_start(ap, format);
    rc = (Platform::instance().output())(format, ap);
    va_end(ap);

    return rc;
}


CXXCAPI ssize_t platform_errorf(const char* format, ...) {
    va_list ap;
    ssize_t rc;

    va_start(ap, format);
    rc = (Platform::instance().error())(format, ap);
    va_end(ap);

    return rc;
}


CXXCAPI int* platform_errornumber() {
    return Platform::instance().errornumber();
}


CXXCAPI int platform_errormaximum() {
    return Platform::instance().errormaximum();
}


CXXCAPI const char* platform_errormessage(
    int errndx,
    char* buffer,
    size_t buflen
) {
    return Platform::instance().errormessage(errndx, buffer, buflen);
}
