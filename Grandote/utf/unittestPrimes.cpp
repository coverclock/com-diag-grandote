/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2009-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the Primes unit test.
 *
 *  @see    Primes
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Primes.h"
#include "com/diag/desperado/Primes.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

CXXCAPI int unittestPrimes(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    {
        Primes primes(20);
        static const Primes::Type array[] = {
            2, 3, 5, 7, 11, 13, 17, 19
        };
        if (primes.size() != countof(array)) {
            errorf("%s[%d]: (%u!=%u)!\n",
                __FILE__, __LINE__, primes.size(), countof(array));
            ++errors;
        }
        Primes::Iterator here = primes.begin();
        Primes::Iterator end = primes.end();
        for (size_t index = 0; index < countof(array); ++index) {
            if (here == end) {
                errorf("%s[%d]: [%u] end!\n",
                    __FILE__, __LINE__, index);
                ++errors;
                break;
            } else if (*here != array[index]) {
                errorf("%s[%d]: [%u] (%d!=%d)!\n",
                    __FILE__, __LINE__, index, *here, array[index]);
                ++errors;
            }
            ++here;
        }
    }

    {
        Primes primes(100);
        static const Primes::Type array[] = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
            53, 59, 61, 67, 71, 73, 79, 83, 89, 97
        };
        if (primes.size() != countof(array)) {
            errorf("%s[%d]: (%u!=%u)!\n",
                __FILE__, __LINE__, primes.size(), countof(array));
            ++errors;
        }
        Primes::Iterator here = primes.begin();
        Primes::Iterator end = primes.end();
        for (size_t index = 0; index < countof(array); ++index) {
            if (here == end) {
                errorf("%s[%d]: [%u] end!\n",
                    __FILE__, __LINE__, index);
                ++errors;
                break;
            } else if (*here != array[index]) {
                errorf("%s[%d]: [%u] (%d!=%d)!\n",
                    __FILE__, __LINE__, index, *here, array[index]);
                ++errors;
            }
            ++here;
        }
    }

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__,
        errors);

    return errors;
}
