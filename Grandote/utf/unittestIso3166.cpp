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
 *  Implements the Iso3166 unit test.
 *
 *  @see    Iso3166
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Iso3166.h"
#include "com/diag/desperado/Iso3166.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

static Iso3166 staticIso3166;

CXXCAPI int unittestIso3166(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticIso3166.show();

    Iso3166 iso3166;
    iso3166.show();

    Iso3166 iso31662 = iso3166;
    iso31662.show();

    Iso3166 iso31663;
    iso31663 = iso3166;
    iso31663.show();

    Iso3166::Index index;
    const Iso3166::Entry* entry;

    printf("%5s %*s %*s %*s %6s\n",
        "INDEX",
        -(sizeof(entry->name) - 1), "NAME",
        sizeof(entry->a2) - 1, "A2",
        sizeof(entry->a3) - 1, "A3",
        "NUMBER");

    for (
        int ii = Iso3166::MINIMUM;
        Iso3166::MAXIMUM >= ii;
        ++ii
    ) {

        index = static_cast<Iso3166::Index>(ii);
        entry = &(iso3166[index]);
        printf("%5d %*s %*s %*s %6d\n",
            index,
            -(sizeof(entry->name) - 1), entry->name,
            sizeof(entry->a2) - 1, entry->a2,
            sizeof(entry->a3) - 1, entry->a3,
            entry->number);

    }

    printf("%5s %*s %*s %*s %6s\n",
        "INDEX",
        -(sizeof(entry->name) - 1), "NAME",
        sizeof(entry->a2) - 1, "A2",
        sizeof(entry->a3) - 1, "A3",
        "NUMBER");

    index = Iso3166::UNITED_STATES;
    entry = &(iso3166[index]);
    printf("%5d %*s %*s %*s %6d\n",
        index,
        -(sizeof(entry->name) - 1), entry->name,
        sizeof(entry->a2) - 1, entry->a2,
        sizeof(entry->a3) - 1, entry->a3,
        entry->number);
    if (0 != std::strcmp(entry->a3, "USA")) {
        errorf("%s[%d]: (\"%s\"!=\"%s\")!\n",
            __FILE__, __LINE__, entry->a3, "USA");
    }

    index = Iso3166::N_124;
    entry = &(iso3166[index]);
    printf("%5d %*s %*s %*s %6d\n",
        index,
        -(sizeof(entry->name) - 1), entry->name,
        sizeof(entry->a2) - 1, entry->a2,
        sizeof(entry->a3) - 1, entry->a3,
        entry->number);
    if (0 != std::strcmp(entry->a3, "CAN")) {
        errorf("%s[%d]: (\"%s\"!=\"%s\")!\n",
            __FILE__, __LINE__, entry->a3, "CAN");
    }

    index = Iso3166::A2_FR;
    entry = &(iso3166[index]);
    printf("%5d %*s %*s %*s %6d\n",
        index,
        -(sizeof(entry->name) - 1), entry->name,
        sizeof(entry->a2) - 1, entry->a2,
        sizeof(entry->a3) - 1, entry->a3,
        entry->number);
    if (0 != std::strcmp(entry->a3, "FRA")) {
        errorf("%s[%d]: (\"%s\"!=\"%s\")!\n",
            __FILE__, __LINE__, entry->a3, "FRA");
    }

    index = Iso3166::A3_GBR;
    entry = &(iso3166[index]);
    printf("%5d %*s %*s %*s %6d\n",
        index,
        -(sizeof(entry->name) - 1), entry->name,
        sizeof(entry->a2) - 1, entry->a2,
        sizeof(entry->a3) - 1, entry->a3,
        entry->number);
    if (0 != std::strcmp(entry->a3, "GBR")) {
        errorf("%s[%d]: (\"%s\"!=\"%s\")!\n",
            __FILE__, __LINE__, entry->a3, "GBR");
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
