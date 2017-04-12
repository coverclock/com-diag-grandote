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
 *  Implements the Root unit test.
 *
 *  @see    Root
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Root.h"
#include "com/diag/desperado/Root.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

static int test(unsigned int value, unsigned int actual, unsigned int expected) {
    Print errorf(Platform::instance().error());
    int errors = 0;
    if (actual != expected) {
        errorf("%s[%d]: 0x%08x (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, value, actual, expected);
        ++errors;
    }
    return errors;
}

CXXCAPI int unittestRoot(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    printf("%s[%d]: root\n", __FILE__, __LINE__);

    errors += test(0x00000000, root(0x00000000), 0x00000000);
    errors += test(0x00000001, root(0x00000001), 0x00000001);
    errors += test(0x00000002, root(0x00000002), 0x00000001);
    errors += test(0x00000004, root(0x00000004), 0x00000003);
    errors += test(0x00000008, root(0x00000008), 0x00000003);
    errors += test(0x00000010, root(0x00000010), 0x00000007);
    errors += test(0x00000020, root(0x00000020), 0x00000007);
    errors += test(0x00000040, root(0x00000040), 0x0000000f);
    errors += test(0x00000080, root(0x00000080), 0x0000000f);
    errors += test(0xffffffff, root(0xffffffff), 0x0000ffff);
    errors += test(0x7fffffff, root(0x7fffffff), 0x0000ffff);
    errors += test(0x3fffffff, root(0x3fffffff), 0x00007fff);
    errors += test(0x1fffffff, root(0x1fffffff), 0x00007fff);
    errors += test(0x0fffffff, root(0x0fffffff), 0x00003fff);
    errors += test(0x07ffffff, root(0x07ffffff), 0x00003fff);
    errors += test(0x03ffffff, root(0x03ffffff), 0x00001fff);
    errors += test(0x01ffffff, root(0x01ffffff), 0x00001fff);
    errors += test(0x00ffffff, root(0x00ffffff), 0x00000fff);

    printf("%s[%d]: minimum\n", __FILE__, __LINE__);

    errors += test(0, minimum(1, 0), 0);
    errors += test(1, minimum(0, 1), 0);
    errors += test(2, minimum(1, 1), 1);
    errors += test(3, minimum(1, -1), -1);
    errors += test(4, minimum(-1, 1), -1);
    errors += test(5, minimum(-1, -1), -1);

    printf("%s[%d]: maximum\n", __FILE__, __LINE__);

    errors += test(0, maximum(1, 0), 1);
    errors += test(1, maximum(0, 1), 1);
    errors += test(2, maximum(1, 1), 1);
    errors += test(3, maximum(1, -1), 1);
    errors += test(4, maximum(-1, 1), 1);
    errors += test(5, maximum(-1, -1), -1);

    printf("%s[%d]: absolute\n", __FILE__, __LINE__);

    errors += test(0, absolute(0), 0);
    errors += test(1, absolute(1), 1);
    errors += test(-1, absolute(-1), 1);

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__,
        errors);

    return errors;
}

