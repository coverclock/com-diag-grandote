/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Grandote library.
    
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


#include "com/diag/grandote/UnitTest.h"
#include "com/diag/grandote/Logger.h"
#include "com/diag/grandote/Output.h"
#include "com/diag/grandote/Output.h"
#include "com/diag/grandote/Print.h"
#include "com/diag/grandote/Print.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/Platform.h"

int unittestLogger2() {
    int errors = 0;
    ssize_t rc;

    print_f(platform_output(), "%s[%d]: begin\n",
        __FILE__, __LINE__);

#define GRANDOTE_LOGGER_TEST_6(_FUNCTION_) \
    rc = logger_##_FUNCTION_(platform_logger(), "%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "cxxcapi", #_FUNCTION_); \
    if (0 >= rc) { \
        print_f(platform_error(), "%s[%d]: (%d>=%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    GRANDOTE_LOGGER_TEST_6(finest)
    GRANDOTE_LOGGER_TEST_6(finer)
    GRANDOTE_LOGGER_TEST_6(fine)
    GRANDOTE_LOGGER_TEST_6(trace)
    GRANDOTE_LOGGER_TEST_6(debug)
    GRANDOTE_LOGGER_TEST_6(information)
    GRANDOTE_LOGGER_TEST_6(configuration)
    GRANDOTE_LOGGER_TEST_6(notice)
    GRANDOTE_LOGGER_TEST_6(warning)
    GRANDOTE_LOGGER_TEST_6(error)
    GRANDOTE_LOGGER_TEST_6(severe)
    GRANDOTE_LOGGER_TEST_6(critical)
    GRANDOTE_LOGGER_TEST_6(alert)
    GRANDOTE_LOGGER_TEST_6(fatal)
    GRANDOTE_LOGGER_TEST_6(emergency)
    GRANDOTE_LOGGER_TEST_6(print)

    print_f(platform_output(), "%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
