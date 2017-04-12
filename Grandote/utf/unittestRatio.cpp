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
 *  Implements the Ratio unit test.
 *
 *  @see    Ratio
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Ratio.h"
#include "com/diag/desperado/Ratio.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

template <typename _TYPE_>
static int convert(const Ratio & ratio)
{
    Print errorf(Platform::instance().error());
    int errors = 0;
    if (ratio.denominator() != 0) {
        _TYPE_ actual;
        actual = ratio.numerator();
        actual /= ratio.denominator();
        _TYPE_ expected;
        expected = ratio;
        if (actual != expected) {
            errorf("%s[%d]: conversion!\n",
                __FILE__, __LINE__);
            ++errors;
        }
    }
    return errors;
}

static int test(const Ratio & ratio, int numerator, int denominator) {
    Print errorf(Platform::instance().error());
    int errors = 0;
    if (ratio.numerator() != numerator) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__,
            ratio.numerator(), numerator);
        ++errors;
    }
    if (ratio.denominator() != denominator) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__,
            ratio.denominator(), denominator);
        ++errors;
    }
    errors += convert<int>(ratio);
    errors += convert<long>(ratio);
    errors += convert<long long>(ratio);
    errors += convert<float>(ratio);
    errors += convert<double>(ratio);
    return errors;
}

static int compare(const Ratio & ratio1, const Ratio & ratio2, int expected) {
    Print errorf(Platform::instance().error());
    int errors = 0;
    if ((ratio1 > ratio2) && (expected <= 0)) {
        errorf("%s[%d]: ((%d/%d)>(%d/%d))!\n",
            __FILE__, __LINE__,
            ratio1.numerator(), ratio1.denominator(),
            ratio2.numerator(), ratio2.denominator());
        ++errors;
    }
    if ((ratio1 >= ratio2) && (expected < 0)) {
        errorf("%s[%d]: ((%d/%d)>=(%d/%d))!\n",
            __FILE__, __LINE__,
            ratio1.numerator(), ratio1.denominator(),
            ratio2.numerator(), ratio2.denominator());
        ++errors;
    }
    if ((ratio1 == ratio2) && (expected != 0)) {
        errorf("%s[%d]: ((%d/%d)==(%d/%d))!\n",
            __FILE__, __LINE__,
            ratio1.numerator(), ratio1.denominator(),
            ratio2.numerator(), ratio2.denominator());
        ++errors;
    }
    if ((ratio1 != ratio2) && (expected == 0)) {
        errorf("%s[%d]: !((%d/%d)!=(%d/%d))!\n",
            __FILE__, __LINE__,
            ratio1.numerator(), ratio1.denominator(),
            ratio2.numerator(), ratio2.denominator());
        ++errors;
    }
    if ((ratio1 < ratio2) && (expected >= 0)) {
        errorf("%s[%d]: !((%d/%d)<(%d/%d))!\n",
            __FILE__, __LINE__,
            ratio1.numerator(), ratio1.denominator(),
            ratio2.numerator(), ratio2.denominator());
        ++errors;
    }
    if ((ratio1 <= ratio2) && (expected > 0)) {
        errorf("%s[%d]: !((%d/%d)<=(%d/%d))!\n",
            __FILE__, __LINE__,
            ratio1.numerator(), ratio1.denominator(),
            ratio2.numerator(), ratio2.denominator());
        ++errors;
    }
    return errors;
}

CXXCAPI int unittestRatio(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    printf("%s[%d]: sanity\n", __FILE__, __LINE__);
    { Ratio ratio; errors += test(ratio, 0, 1); }
    { Ratio ratio(24, 8); errors += test(ratio, 3, 1); }
    { Ratio ratio(24, 48); errors += test(ratio, 1, 2); }
    { Ratio ratio(1, 1); errors += test(ratio, 1, 1); }
    { Ratio ratio(-1, 1); errors += test(ratio, -1, 1); }
    { Ratio ratio(1, -1); errors += test(ratio, 1, -1); }
    { Ratio ratio(-1, -1); errors += test(ratio, -1, -1); }
    { Ratio ratio(0, 2); errors += test(ratio, 0, 1); }
    { Ratio ratio(1, 0); errors += test(ratio, 1, 0); }
    { Ratio ratio(2, 1); errors += test(ratio, 2, 1); }
    { Ratio ratio(1, 2); errors += test(ratio, 1, 2); }
    { Ratio ratio(2, 2); errors += test(ratio, 1, 1); }
    { Ratio ratio(-2, 2); errors += test(ratio, -1, 1); }
    { Ratio ratio(2, -2); errors += test(ratio, 1, -1); }
    { Ratio ratio(-2, -2); errors += test(ratio, -1, -1); }
    { Ratio ratio(8, 2); errors += test(ratio, 4, 1); }
    { Ratio ratio(8, -2); errors += test(ratio, 4, -1); }
    { Ratio ratio(-8, 2); errors += test(ratio, -4, 1); }
    { Ratio ratio(-8, -2); errors += test(ratio, -4, -1); }

    {
        printf("%s[%d]: addition\n", __FILE__, __LINE__);
        Ratio ratio0;
        Ratio ratio1(10, 20);
        Ratio ratio2(10, 30);
        ratio0 = ratio1 + ratio2;
        test(ratio0, 5, 6);
    }

    {
        printf("%s[%d]: subtraction\n", __FILE__, __LINE__);
        Ratio ratio0;
        Ratio ratio1(10, 20);
        Ratio ratio2(10, 30);
        ratio0 = ratio1 - ratio2;
        test(ratio0, 1, 6);
    }

    {
        printf("%s[%d]: multiplication\n", __FILE__, __LINE__);
        Ratio ratio0;
        Ratio ratio1(10, 40);
        Ratio ratio2(10, 30);
        ratio0 = ratio1 * ratio2;
        test(ratio0, 1, 12);
    }

    {
        printf("%s[%d]: division\n", __FILE__, __LINE__);
        Ratio ratio0;
        Ratio ratio1(10, 40);
        Ratio ratio2(10, 30);
        ratio0 = ratio1 / ratio2;
        test(ratio0, 3, 4);
    }

    printf("%s[%d]: comparison\n", __FILE__, __LINE__);
    {
        Ratio ratio1(1, 4);
        Ratio ratio2(2, 8);
        errors += compare(ratio1, ratio2, 0);
    }
    {
        Ratio ratio1(1, 4);
        Ratio ratio2(2, 4);
        errors += compare(ratio1, ratio2, -1);
    }
    {
        Ratio ratio1(3, 4);
        Ratio ratio2(2, 4);
        errors += compare(ratio1, ratio2, 1);
    }
    {
        Ratio ratio1(2, 4);
        Ratio ratio2(2, 3);
        errors += compare(ratio1, ratio2, -1);
    }
    {
        Ratio ratio1(5, 6);
        Ratio ratio2(2, 5);
        errors += compare(ratio1, ratio2, 1);
    }

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__, errors);

    return errors;
}
