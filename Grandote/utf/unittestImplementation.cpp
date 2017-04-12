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
 *  Implements the Implementation unit test.
 *
 *  @see    Implementation
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */

#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Implementation.h"
#include "com/diag/desperado/Implementation.h"
#include "com/diag/desperado/Derivation.h"
#include "com/diag/desperado/Derivation.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Desperado.h"

static Implementation implementation;

static Derivation derivation;
static Derivation derivation2 = derivation;
static Derivation derivation3;

CXXCAPI int unittestImplementation(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    printf("%s[%d]: static implementation\n", __FILE__, __LINE__);

    ::implementation.show();

    printf("%s[%d]: stack implementation\n", __FILE__, __LINE__);

    Implementation implementation;
    implementation.show();

    printf("%s[%d]: static derivation\n", __FILE__, __LINE__);

    ::derivation.show();

    printf("%s[%d]: static derivation via base\n", __FILE__, __LINE__);

    Implementation* pi = &::derivation;
    pi->show();

    printf("%s[%d]: static derivation copy construction\n", __FILE__, __LINE__);

    ::derivation2.show();

    printf("%s[%d]: static derivation assignment\n", __FILE__, __LINE__);

    ::derivation3.show();
    ::derivation3 = ::derivation;
    ::derivation3.show();

    printf("%s[%d]: stack derivation\n", __FILE__, __LINE__);

    Derivation derivation;
    derivation.show();

    printf("%s[%d]: stack derivation via base\n", __FILE__, __LINE__);

    pi = &derivation;
    pi->show();

    printf("%s[%d]: stack derivation copy construction\n", __FILE__, __LINE__);

    Derivation derivation2 = derivation;
    derivation2.show();

    printf("%s[%d]: stack derivation assignment\n", __FILE__, __LINE__);

    Derivation derivation3;
    derivation3.show();
    derivation3 = derivation;
    derivation3.show();

    printf("%s[%d]: heap derivation\n", __FILE__, __LINE__);

    Derivation* derivation4 = new Derivation;
    derivation4->show();

    printf("%s[%d]: heap derivation via base\n", __FILE__, __LINE__);

    pi = derivation4;
    pi->show();

    delete derivation4;

    printf("%s[%d]: heap derivation copy construction\n", __FILE__, __LINE__);

    Derivation* derivation5 = new Derivation(derivation);
    derivation5->show();
    delete derivation5;

    printf("%s[%d]: heap derivation assignment\n", __FILE__, __LINE__);

    Derivation* derivation6 = new Derivation;
    derivation6->show();
    *derivation6 = derivation;
    derivation6->show();
    delete derivation6;

    printf("%s[%d]: construction 1 trace\n", __FILE__, __LINE__);

    Derivation* derivation7 = new Derivation;

    printf("%s[%d]: construction 2 trace\n", __FILE__, __LINE__);

    void* pointer = reinterpret_cast<void*>(1);
    Derivation* derivation8 = new Derivation(pointer);

    printf("%s[%d]: copy construction trace\n", __FILE__, __LINE__);

    Derivation* derivation9 = new Derivation(derivation8);

    printf("%s[%d]: assignment trace\n", __FILE__, __LINE__);

    *derivation7 = *derivation9;

    printf("%s[%d]: initialize 3 trace\n", __FILE__, __LINE__);

    Abstract* abstraction7 = derivation7;

    printf("%s[%d]: destruction 1 trace\n", __FILE__, __LINE__);

    delete abstraction7;

    printf("%s[%d]: destruction 2 trace\n", __FILE__, __LINE__);

    delete derivation8;

    printf("%s[%d]: destruction 1 trace\n", __FILE__, __LINE__);

    delete derivation9;

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
