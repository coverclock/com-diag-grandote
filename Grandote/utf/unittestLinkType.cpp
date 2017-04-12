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
 *  Implements the LinkType unit test.
 *
 *  The Chain unit test is a comprehensive unit test of the Chain and
 *  base Link classes. This unit test merely attempts to verify
 *  that the LinkType class appears to be sane.
 *
 *  @see    LinkType
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/LinkType.h"
#include "com/diag/desperado/LinkType.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Desperado.h"

class UT_LinkType {
public:
    UT_LinkType(int n = 0) : sn(n), node(this) {}
    virtual ~UT_LinkType() {}
    int sn;
    LinkType<UT_LinkType> node;
};

template class LinkType<UT_LinkType>;

typedef LinkType<UT_LinkType> UTLink;

class UT_LinkType_Dump : public UTLink::Functor {
public:
    UT_LinkType_Dump(bool f = true) : origin(0), forwards(f) {}
    virtual ~UT_LinkType_Dump() {}
    virtual UTLink* operator() (UTLink* link) {
        if (link == origin) { return 0; }
        if (origin == 0) { origin = link; }
        link->show();
        UTLink* next = link->getNext();
        UTLink* previous = link->getPrevious();
        UT_LinkType* object = link->getPayload();
        if (0 != object) {
            Print printf(Platform::instance().output());
            printf("   UT_LinkType[%p]: sn=%d\n", object, object->sn);
        }
        link = forwards ? next : previous;
        return (link == origin) ? 0 : link;
    }
    UTLink* origin;
    bool forwards;
};

static UT_LinkType* links[10];
static const int limit = countof(links);

CXXCAPI int unittestLinkType(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    UT_LinkType thing[10];
    UT_LinkType* that = 0;
    UT_LinkType_Dump forward;
    UT_LinkType_Dump backward(false);

    printf("%s[%d]: initial forward\n", __FILE__, __LINE__);

    for (size_t ii = 0; countof(thing) > ii; ++ii) {
        printf("%s[%d]: thing[%u]\n", __FILE__, __LINE__, ii);
        thing[ii].node.apply(forward);
    }

    printf("%s[%d]: initial backward\n", __FILE__, __LINE__);

    for (int ii = countof(thing); 0 < ii; --ii) {
        printf("%s[%d]: thing[%u]\n", __FILE__, __LINE__, ii - 1);
        thing[ii - 1].node.apply(backward);
    }

    printf("%s[%d]: insert\n", __FILE__, __LINE__);

    for (size_t ii = 0; countof(thing) > ii; ++ii) {
        thing[ii].sn = ii;
        if (0 != that) {
            thing[ii].node.insert(&that->node);
        }
        that = &thing[ii];
        printf("%s[%d]: thing[%u]\n", __FILE__, __LINE__, 0);
        thing[0].node.apply(forward);
    }

    printf("%s[%d]: audit\n", __FILE__, __LINE__);

    for (size_t ii = 0; countof(thing) > ii; ++ii) {
        printf("%s[%d]: thing[%u]\n", __FILE__, __LINE__, ii);
        if (thing[ii].sn != static_cast<int>(ii)) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__,
                thing[ii].sn, static_cast<int>(ii));
            thing[ii].node.show();
            ++errors;
        }
        if (thing[ii].node.getPayload() != &thing[ii]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__,
                thing[ii].node.getPayload(), &thing[ii]);
            thing[ii].node.show();
            ++errors;
        }
        if (thing[ii].node.isChained() != true) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__,
                thing[ii].node.isChained(), true);
            thing[ii].node.show();
            ++errors;
        }
        if (thing[ii].node.hasRoot(&thing[0].node) != true) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__,
                thing[ii].node.hasRoot(&thing[0].node), true);
            thing[ii].node.show();
            ++errors;
        }
        if (thing[ii].node.getRoot() != &thing[0].node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__,
                thing[ii].node.getRoot(), &thing[0].node);
            thing[ii].node.show();
            ++errors;
        }
        if (thing[ii].node.getNext() != &thing[(ii + 1) % 10].node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__,
                thing[ii].node.getNext(), &thing[(ii + 1) % 10].node);
            thing[ii].node.show();
            ++errors;
        }
        if (thing[ii].node.getPrevious() != &thing[(ii + 9) % 10].node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__,
                thing[ii].node.getPrevious(), &thing[(ii + 9) % 10].node);
            thing[ii].node.show();
            ++errors;
        }
    }

    printf("%s[%d]: first forward\n", __FILE__, __LINE__);

    thing[0].node.apply(forward);

    printf("%s[%d]: first backward\n", __FILE__, __LINE__);

    thing[0].node.apply(backward);

    printf("%s[%d]: last forward\n", __FILE__, __LINE__);

    thing[countof(thing) - 1].node.apply(forward);

    printf("%s[%d]: last backward\n", __FILE__, __LINE__);

    thing[countof(thing) - 1].node.apply(backward);

    printf("%s[%d]: remove\n", __FILE__, __LINE__);

    for (size_t ii = countof(thing); 0 < ii; --ii) {
        thing[ii - 1].node.remove();
        printf("%s[%d]: thing[%u]\n", __FILE__, __LINE__, 0);
        thing[0].node.apply(forward);
        printf("%s[%d]: thing[%u]\n", __FILE__, __LINE__, ii - 1);
        thing[ii - 1].node.apply(forward);
    }

    printf("%s[%d]: final forward\n", __FILE__, __LINE__);

    for (size_t ii = 0; countof(thing) > ii; ++ii) {
        printf("%s[%d]: thing[%u]\n", __FILE__, __LINE__, ii);
        thing[ii].node.apply(forward);
    }

    printf("%s[%d]: final backward\n", __FILE__, __LINE__);

    for (size_t ii = countof(thing); 0 < ii; --ii) {
        printf("%s[%d]: thing[%u]\n", __FILE__, __LINE__, ii - 1);
        thing[ii - 1].node.apply(backward);
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
