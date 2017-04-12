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
 *  Implements the Meter unit test.
 *
 *  @see    Meter
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Meter.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Desperado.h"

static int mean(Meter& meter) {
    Print printf(Platform::instance().output());
    int errors = 0;

    printf("mean(%p)\n", &meter);
    printf("\tmean=(%lu/%lu)=(%lu<=%lu<=%lu<=%lu<=%lu)\n",
        meter.getNumerator(), meter.getDenominator(),
        meter.getMinimum(),
        meter.mean(-1), meter.mean(0), meter.mean(1),
        meter.getMaximum());
    printf("\tsampled=%lu scaled=%lu\n",
        meter.getSampled(), meter.getScaled());

    return errors;
}

static int sample(Meter& meter, size_t n, size_t d) {
    Print printf(Platform::instance().output());
    int errors = 0;

    printf("sample(%p,%lu,%lu)\n", &meter, n, d);
    size_t ratio = meter.sample(n, d);
    printf("\tratio=(%lu/%lu)=(%lu<=%lu<=%lu)\n",
        n, d, meter.getMinimum(), ratio, meter.getMaximum());
    errors += mean(meter);

    return errors;
}

static Meter staticMeter;

CXXCAPI int unittestMeter(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticMeter.show();

    Meter meter1;
    meter1.show();

    Meter meter3 = meter1;
    meter3.show();

    Meter meter4;
    meter4 = meter1;
    meter4.show();

    printf("%s[%d]: limits\n", __FILE__, __LINE__);

    errors += mean(meter1);

    errors += sample(meter1, 0, 0);
    errors += sample(meter1, 0, 1);
    errors += sample(meter1, 1, 0);
    errors += sample(meter1, 1, 1);
    errors += sample(meter1, 0, ~0U);
    errors += sample(meter1, ~0U, 0);
    errors += sample(meter1, ~0U, ~0U);

    meter1.show();

    printf("%s[%d]: reset\n", __FILE__, __LINE__);

    meter1.reset();

    meter1.show();

    printf("%s[%d]: sanity\n", __FILE__, __LINE__);

    errors += sample(meter1, 12, 4);
    errors += sample(meter1, 6, 4);
    errors += sample(meter1, 2, 2);

    meter1.reset();

    printf("%s[%d]: overflow\n", __FILE__, __LINE__);

    for (int ii = 0; 10 > ii; ++ii) {
        errors += sample(meter1, ~0U, ~0U);
    }

    meter1.show();

    printf("%s[%d]: mean\n", __FILE__, __LINE__);

    Meter meter2;

    errors += mean(meter2);

    for (int ii = 0; 1000000 > ii; ++ii) {
        meter2.sample(12, 4);
        meter2.sample(6, 4);
        meter2.sample(2, 2);
    }

    errors += mean(meter2);

    for (int ii = 0; 1000000 > ii; ++ii) {
        meter2.sample(12000000, 4000000);
        meter2.sample(6000000, 4000000);
        meter2.sample(2000000, 2000000);
    }

    errors += mean(meter2);

    for (size_t uu = 1; 2000000000 > uu; ++uu) {
        meter2.sample(uu * 2, uu);
    }

    errors += mean(meter2);

    meter2.show();

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
