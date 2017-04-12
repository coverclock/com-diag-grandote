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
 *  Implements the Argument unit test.
 *
 *  @see    Argument
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Argument.h"
#include "com/diag/desperado/Argument.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

static Argument staticArgument;

CXXCAPI int unittestArgument(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticArgument.show();

    Argument argument;
    argument.show();

    Argument argument2 = argument;
    argument2.show();

    Argument argument3;
    argument3 = argument;
    argument3.show();

    const char* string = "Every good boy does fine.";
    const char* alternate = "Every&good&boy&does&fine.";
    const char* tokens[] = { "Every", "good", "boy", "does", "fine." };
    const size_t MAXIMUM = sizeof(tokens)/sizeof(tokens[0]);
    int argc;
    char* argv[MAXIMUM + 2];
    int expected;
    int actual;
    char buffer[256];
    char empty[1] = { '\0' };

    printf("%s[%d]: default delimiters\n", __FILE__, __LINE__);

    printf("%4s %4s %4s \"%s\"\n",
        "MAX", "ARGC", "II", string);
    for (size_t ii = 0; (sizeof(argv)/sizeof(argv[0])) > ii; ++ii) {
        ::strncpy(buffer, string, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';
        for (size_t jj = 0; (sizeof(argv)/sizeof(argv[0])) > jj; ++jj) {
            argv[jj] = empty;
        }
        expected = (MAXIMUM > ii) ? ii : MAXIMUM;
        argc = argument.vector(buffer, argv, ii);
        if (expected != argc) {
            errorf("%s[%d]: (%d!=%d[%d])!\n",
                __FILE__, __LINE__, expected, argc, ii);
            ++errors;
        }
        actual = (static_cast<int>(MAXIMUM) > argc)
            ? argc : static_cast<int>(MAXIMUM);
        for (int jj = 0; jj < actual; ++jj) {
            if (::strcmp(tokens[jj], argv[jj]) != 0) {
                errorf("%s[%d]: (\"%s\"!=\"%s\"[%d][%d])!\n",
                    __FILE__, __LINE__, tokens[jj], argv[jj], ii, jj);
                ++errors;
            }
        }
        if (argc < static_cast<int>(ii)) {
            if (0 != argv[argc]) {
                errorf("%s[%d]: (%p!=%p[%d][%d])!\n",
                    __FILE__, __LINE__, 0, argv[argc], ii, argc);
                ++errors;
            }
        }
        for (size_t jj = argc + 1; (sizeof(argv)/sizeof(argv[0])) > jj; ++jj){
            if (empty != argv[jj]) {
                errorf("%s[%d]: (%p!=%p[%d][%d])!\n",
                    __FILE__, __LINE__, empty, argv[jj], ii, jj);
                ++errors;
            }
        }
        for (size_t jj = 0; (sizeof(argv)/sizeof(argv[0])) > jj; ++jj) {
            printf("%4d %4d %4d %s%s%s\n",
                ii, argc, jj,
                (0 != argv[ii]) ? "\"" : "",
                (0 != argv[ii]) ? argv[jj] : "(NULL)",
                (0 != argv[ii]) ? "\"" : "");
        }
    }

    printf("%s[%d]: other delimiters\n", __FILE__, __LINE__);

    printf("%4s %4s %4s \"%s\"\n",
        "MAX", "ARGC", "II", alternate);
    for (size_t ii = 0; (sizeof(argv)/sizeof(argv[0])) > ii; ++ii) {
        ::strncpy(buffer, alternate, sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';
        for (size_t jj = 0; (sizeof(argv)/sizeof(argv[0])) > jj; ++jj) {
            argv[jj] = empty;
        }
        expected = (MAXIMUM > ii) ? ii : MAXIMUM;
        argc = argument.vector(buffer, argv, ii, "&");
        if (expected != argc) {
            errorf("%s[%d]: (%d!=%d[%d])!\n",
                __FILE__, __LINE__, expected, argc, ii);
            ++errors;
        }
        actual = (static_cast<int>(MAXIMUM) > argc)
            ? argc : static_cast<int>(MAXIMUM);
        for (int jj = 0; jj < actual; ++jj) {
            if (::strcmp(tokens[jj], argv[jj]) != 0) {
                errorf("%s[%d]: (\"%s\"!=\"%s\"[%d][%d])!\n",
                    __FILE__, __LINE__, tokens[jj], argv[jj], ii, jj);
                ++errors;
            }
        }
        if (argc < static_cast<int>(ii)) {
            if (0 != argv[argc]) {
                errorf("%s[%d]: (%p!=%p[%d][%d])!\n",
                    __FILE__, __LINE__, 0, argv[argc], ii, argc);
                ++errors;
            }
        }
        for (size_t jj = argc + 1; (sizeof(argv)/sizeof(argv[0])) > jj; ++jj) {
            if (empty != argv[jj]) {
                errorf("%s[%d]: (%p!=%p[%d][%d])!\n",
                    __FILE__, __LINE__, empty, argv[jj], ii, jj);
                ++errors;
            }
        }
        for (size_t jj = 0; (sizeof(argv)/sizeof(argv[0])) > jj; ++jj) {
            printf("%4d %4d %4d %s%s%s\n",
                ii, argc, jj,
                (0 != argv[ii]) ? "\"" : "",
                (0 != argv[ii]) ? argv[jj] : "(NULL)",
                (0 != argv[ii]) ? "\"" : "");
        }
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
