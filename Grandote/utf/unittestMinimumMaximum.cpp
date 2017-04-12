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
 *  Implements the MinimumMaximum unit test.
 *
 *  @see    MinimumMaximum
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Minimum.h"
#include "com/diag/desperado/Maximum.h"
#include "com/diag/desperado/Maximum.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Desperado.h"

static Minimum<int> staticMinimum;
static Maximum<int> staticMaximum;

Minimum<size_t> minimum;
Maximum<size_t> maximum;

CXXCAPI int unittestMinimumMaximum(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    printf("%s[%d]: staticMinimum=%p\n", &::staticMinimum);
    printf("%s[%d]: staticMaximum=%p\n", &::staticMaximum);

    size_t result;
    size_t expected;

    result = minimum(unsignedintminof(size_t));
    expected = unsignedintminof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(unsignedintmaxof(size_t));
    expected = unsignedintmaxof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(unsignedintminof(size_t), unsignedintmaxof(size_t));
    expected = unsignedintminof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(unsignedintmaxof(size_t), unsignedintminof(size_t));
    expected = unsignedintminof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(unsignedintminof(size_t), unsignedintminof(size_t));
    expected = unsignedintminof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(unsignedintmaxof(size_t), unsignedintmaxof(size_t));
    expected = unsignedintmaxof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(0, 1);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(0, 1, 2);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 0, 2);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(0, 1, 2, 3);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 0, 2, 3);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 0, 3);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(0, 1, 2, 3, 4);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 0, 2, 3, 4);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 0, 3, 4);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 0, 4);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(0, 1, 2, 3, 4, 5);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 0, 2, 3, 4, 5);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 0, 3, 4, 5);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 0, 4, 5);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 0, 5);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 5, 0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(0, 1, 2, 3, 4, 5, 6);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 0, 2, 3, 4, 5, 6);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 0, 3, 4, 5, 6);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 0, 4, 5, 6);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 0, 5, 6);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 5, 0, 6);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 5, 6, 0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(0, 1, 2, 3, 4, 5, 6, 7);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 0, 2, 3, 4, 5, 6, 7);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 0, 3, 4, 5, 6, 7);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 0, 4, 5, 6, 7);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 0, 5, 6, 7);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 5, 0, 6, 7);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 5, 6, 0, 7);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = minimum(1, 2, 3, 4, 5, 6, 7, 0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(unsignedintminof(size_t));
    expected = unsignedintminof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(unsignedintmaxof(size_t));
    expected = unsignedintmaxof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(unsignedintminof(size_t), unsignedintmaxof(size_t));
    expected = unsignedintmaxof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(unsignedintmaxof(size_t), unsignedintminof(size_t));
    expected = unsignedintmaxof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(unsignedintminof(size_t), unsignedintminof(size_t));
    expected = unsignedintminof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(unsignedintmaxof(size_t), unsignedintmaxof(size_t));
    expected = unsignedintmaxof(size_t);
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0);
    expected = 0;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1);
    expected = 1;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(1, 0);
    expected = 1;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2);
    expected = 2;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 2, 1);
    expected = 2;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(2, 0, 1);
    expected = 2;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3);
    expected = 3;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 3, 2);
    expected = 3;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 3, 1, 2);
    expected = 3;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(3, 0, 1, 2);
    expected = 3;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 4);
    expected = 4;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 4, 3);
    expected = 4;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 4, 2, 3);
    expected = 4;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 4, 1, 2, 3);
    expected = 4;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(4, 0, 1, 2, 3);
    expected = 4;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 4, 5);
    expected = 5;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 5, 4);
    expected = 5;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 5, 3, 4);
    expected = 5;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 5, 2, 3, 4);
    expected = 5;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 5, 1, 2, 3, 4);
    expected = 5;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(5, 0, 1, 2, 3, 4);
    expected = 5;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 4, 5, 6);
    expected = 6;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 4, 6, 5);
    expected = 6;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 6, 4, 5);
    expected = 6;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 6, 3, 4, 5);
    expected = 6;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 6, 2, 3, 4, 5);
    expected = 6;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 6, 1, 2, 3, 4, 5);
    expected = 6;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(6, 0, 1, 2, 3, 4, 5);
    expected = 6;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 4, 5, 6, 7);
    expected = 7;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 4, 5, 7, 6);
    expected = 7;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 4, 7, 5, 6);
    expected = 7;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 3, 7, 4, 5, 6);
    expected = 7;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 2, 7, 3, 4, 5, 6);
    expected = 7;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 1, 7, 2, 3, 4, 5, 6);
    expected = 7;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(0, 7, 1, 2, 3, 4, 5, 6);
    expected = 7;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    result = maximum(7, 0, 1, 2, 3, 4, 5, 6);
    expected = 7;
    if (result != expected) {
        errorf("%s[%d]: (%u!=%u)\n",
            __FILE__, __LINE__, result, expected);
        ++errors;
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
