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
 *  Implements the Logger CXXCAPI unit test.
 *
 *  @see    Logger
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"

int unittestLogger2() {
    int errors = 0;
    ssize_t rc;

    print_f(platform_output(), "%s[%d]: begin\n",
        __FILE__, __LINE__);

#define DESPERADO_LOGGER_TEST_6(_FUNCTION_) \
    rc = logger_##_FUNCTION_(platform_logger(), "%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "cxxcapi", #_FUNCTION_); \
    if (0 >= rc) { \
        print_f(platform_error(), "%s[%d]: (%d>=%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    DESPERADO_LOGGER_TEST_6(finest)
    DESPERADO_LOGGER_TEST_6(finer)
    DESPERADO_LOGGER_TEST_6(fine)
    DESPERADO_LOGGER_TEST_6(trace)
    DESPERADO_LOGGER_TEST_6(debug)
    DESPERADO_LOGGER_TEST_6(information)
    DESPERADO_LOGGER_TEST_6(configuration)
    DESPERADO_LOGGER_TEST_6(notice)
    DESPERADO_LOGGER_TEST_6(warning)
    DESPERADO_LOGGER_TEST_6(error)
    DESPERADO_LOGGER_TEST_6(severe)
    DESPERADO_LOGGER_TEST_6(critical)
    DESPERADO_LOGGER_TEST_6(alert)
    DESPERADO_LOGGER_TEST_6(fatal)
    DESPERADO_LOGGER_TEST_6(emergency)
    DESPERADO_LOGGER_TEST_6(print)

    print_f(platform_output(), "%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
