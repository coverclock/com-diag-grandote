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
 *  Implements the Throttle unit test.
 *
 *  @see    Throttle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Throttle.h"
#include "com/diag/desperado/Throttle.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

static Throttle staticThrottle;

CXXCAPI int unittestThrottle(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;
    Throttle throttle;
    Throttle save1 = throttle;
    Throttle save2;
    ticks_t delay;
    bool alarmed;
    bool ok;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticThrottle.show();

    printf("%s[%d]: constructors\n", __FILE__, __LINE__);

    throttle.show();
    save1.show();
    save2 = throttle;
    save2.show();

    printf("%s[%d]: initial\n", __FILE__, __LINE__);
    throttle.show();

    printf("%s[%d]: time\n", __FILE__, __LINE__);

    if ((delay = throttle.frequency()) != 0) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, delay, 0);
        throttle.show();
        ++errors;
    }

    printf("%s[%d]: admissibility\n", __FILE__, __LINE__);

    if ((alarmed = throttle.isAlarmed())) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, alarmed, false);
        throttle.show();
        ++errors;
    }

    for (int ii = 1; ii <= 5; ++ii) {

        if ((delay = throttle.admissible()) != 0) {
            errorf("%s[%d]: (%lu!=%lu)!\n",
                __FILE__, __LINE__, delay, 0);
            throttle.show();
            ++errors;
        }

        if (!(ok = throttle.commit())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, ok, true);
            throttle.show();
            ++errors;
        }

        if ((alarmed = throttle.isAlarmed())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, alarmed, false);
            throttle.show();
            ++errors;
        }

    }

    printf("%s[%d]: final\n", __FILE__, __LINE__);

    throttle.show();

    printf("%s[%d]: reset\n", __FILE__, __LINE__);

    throttle.reset();
    throttle.show();

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__, errors);

    return errors;
}
