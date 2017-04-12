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
 *  Implements the namespace unit test.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/cxxcapi.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"

CXXCAPI int unittestnamespace(void);

#include "com/diag/desperado/Begin.h"
static int function1() {
	return 1;
}
#include "com/diag/desperado/End.h"

static int function2() {
	return com::diag::desperado::function1() + 1;
}

#include "com/diag/desperado/Desperado.h"

#include "com/diag/desperado/Begin.h"
static int function3() {
	return function1() + 2;
}
#include "com/diag/desperado/End.h"

#include "com/diag/desperado/Begin.h"
static int function4() {
	return ::function2() + 2;
}
#include "com/diag/desperado/End.h"

CXXCAPI int unittestnamespace(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;
    int actual = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    actual += function1();
    actual += function2();
    actual += function3();
    actual += function4();
    
    int expected = 1 + 2 + 3 + 4;
    if (actual != expected) {
        errorf("%s[%d]: (%d!=%d)\n", __FILE__, __LINE__, actual, expected);
    	++errors;
    }

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__, errors);

    return errors;
}
