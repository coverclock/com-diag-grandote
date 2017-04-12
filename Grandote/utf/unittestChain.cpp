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
 *  Implements the Chain unit test.
 *
 *  @see    Chain
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Link.h"
#include "com/diag/desperado/Link.h"
#include "com/diag/desperado/Chain.h"
#include "com/diag/desperado/Chain.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Desperado.h"

class UT_Chain_Link {
public:
    UT_Chain_Link(int n) : sn(n), node(this) {}
    virtual ~UT_Chain_Link() {}
    int sn;
    Link node;
};

enum Range {
    ONE,
    FORWARD,
    REVERSE
};

class UT_Chain_Functor : public Link::Functor {
public:
    UT_Chain_Functor(Range r) : origin(0), range(r) {}
    virtual ~UT_Chain_Functor() {}
    virtual Link* operator() (Link* link) {
        if (link == origin) { return 0; }
        if (0 == origin) { origin = link; }
        Print printf(Platform::instance().output());
        printf("origin=0x%x link=0x%x Range=%d\n",
            origin, link, range);
        if (range == ONE) { return link; }
        else if (range == FORWARD) { link = link->getNext(); }
        else if (range == REVERSE) { link = link->getPrevious(); }
        return (link == origin) ? 0 : link;
    }
    Link* origin;
    Range range;
};

class UT_Chain_Dump : public Link::Functor {
public:
    UT_Chain_Dump(bool a = true, bool f = true) : origin(0), all(a), forwards(f) {}
    virtual ~UT_Chain_Dump() {}
    virtual Link* operator() (Link* link) {
        if (link == origin) { return 0; }
        if (0 == origin) { origin = link; }
        link->show();
        Link* next = link->getNext();
        Link* previous = link->getPrevious();
        if (link->isRoot()) {
            if (!all) {
                return link;
            }
        } else {
            UT_Chain_Link* object =
                            static_cast<UT_Chain_Link*>(link->getPayload());
            if (0 != object) {
                Print printf(Platform::instance().output());
                printf("  UT_Chain_Link[%p]: sn=%d\n", object, object->sn);
            }
        }
        link = forwards ? next : previous;
        return (link == origin) ? 0 : link;
    }
    virtual void reset() { origin = 0; }
    Link* origin;
    bool all;
    bool forwards;
};

static Chain* chain;
static UT_Chain_Link* links[10];
static const int LIMIT = countof(links);

class UT_Chain_Remove : public Link::Functor {
public:
    UT_Chain_Remove(bool f = true) : origin(0), forwards(f) {}
    virtual ~UT_Chain_Remove() {}
    virtual Link* operator() (Link* link) {
        if (link == origin) { return 0; }
        if (0 == origin) { origin = link; }
        Link* next = link->getNext();
        Link* previous = link->getPrevious();
        if (!(link->isRoot())) {
            link->remove();
        }
        link = forwards ? next : previous;
        return (link == origin) ? 0 : link;
    }
    virtual void reset() { origin = 0; }
    Link* origin;
    bool forwards;
};

class UT_Chain_Insert : public Link::Functor {
public:
    UT_Chain_Insert(bool f = true) : origin(0), forwards(f), index(forwards ? 0 : LIMIT) {}
    virtual ~UT_Chain_Insert() {}
    virtual Link* operator() (Link* link) {
        if (link == origin) { return 0; }
        if (0 == origin) { origin = link; }
        int effective = forwards ? index++ : --index;
        if ((0 <= effective) && (effective < LIMIT)) {
            Link* after = forwards ? link : link->getPrevious();
            links[effective]->node.insert(after);
            Link* next = link->getNext();
            Link* previous = link->getPrevious();
            link = forwards ? next : previous;
        }
        return (link == origin) ? 0 : link;
    }
    virtual void reset() { origin = 0; }
    Link* origin;
    bool forwards;
    int index;
};

static int audit(Chain* chain, const char* file, const int line) {
    Print errorf(Platform::instance().error());
    int errors = 0;

    Link* node;
    Link* root;
    void* payload;
    UT_Chain_Link* object;
    int sn;
    bool success;

    // Root audit.

    root = chain;
    payload = root->getPayload();
    if (0 != payload) {
        errorf("%s[%d]: %s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, file, line, 0, payload);
        ++errors;
    }

    node = root->getRoot();
    if (node != root) {
        errorf("%s[%d]: %s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, file, line, node, root);
        ++errors;
    }

    if ((success = chain->isEmpty())) {
        errorf("%s[%d]: %s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, file, line,
            success, false);
        ++errors;
    }

    // Forward audit.

    node = chain->peekFirst();
    object = (0 == node) ? 0 : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = 0; sn < LIMIT; ++sn) {
        if (node != &(object->node)) {
            errorf("%s[%d]: %s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, file, line, node, &(object->node));
            ++errors;
            break;
        } else if (!chain->isMember(node)) {
            errorf("%s[%d]: %s[%d]: object=%p (%d!=%d)!\n",
                __FILE__, __LINE__, file, line, object,
                chain->isMember(node), true);
            ++errors;
            break;
        } else if (node->getRoot() != root) {
            errorf(
                "%s[%d]: %s[%d]: object=%p (%p!=%p)!\n",
                __FILE__, __LINE__, file, line, object, node->getRoot(), root);
            ++errors;
            break;
        } else if (!(success = node->hasRoot(root))) {
            errorf("%s[%d]: %s[%d]: object=%p (%d!=%d)!\n",
                __FILE__, __LINE__, file, line, object, success, true);
            ++errors;
            break;
        } else if (object != links[sn]) {
            errorf("%s[%d]: %s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, file, line, object, links[sn]);
            ++errors;
            break;
        } else if (object->sn != sn) {
            errorf("%s[%d]: %s[%d]: object=%p (%d!=%d)!\n",
                __FILE__, __LINE__, file, line, object, object, sn);
            ++errors;
            break;
        }
        node = chain->peekNext(node);
        object = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    if (sn != LIMIT) {
        errorf("%s[%d]: %s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, file, line,
            sn, LIMIT);
        ++errors;
    }

    if (0 != node) {
        errorf("%s[%d]: %s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, file, line, node, 0);
        ++errors;
    }

    // Backward audit.

    node = chain->peekLast();
    object = (0 == node) ? 0 : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = LIMIT - 1; 0 <= sn; --sn) {
        if (node != &(object->node)) {
            errorf("%s[%d]: %s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, file, line, node, &(object->node));
            ++errors;
            break;
        } else if (!chain->isMember(node)) {
            errorf("%s[%d]: %s[%d]: object=%p (%d!=%d)!\n",
                __FILE__, __LINE__, file, line, object,
                chain->isMember(node), true);
            ++errors;
            break;
        } else if (node->getRoot() != root) {
            errorf(
                "%s[%d]: %s[%d]: object=%p (%p!=%p)!\n",
                __FILE__, __LINE__, file, line, object, node->getRoot(), root);
            ++errors;
            break;
        } else if (!(success = node->hasRoot(root))) {
            errorf("%s[%d]: %s[%d]: object=%p (%d!=%d)!\n",
                __FILE__, __LINE__, file, line, object, success, true);
            ++errors;
            break;
        } else if (object != links[sn]) {
            errorf("%s[%d]: %s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, file, line, object, links[sn]);
            ++errors;
            break;
        } else if (object->sn != sn) {
            errorf("%s[%d]: %s[%d]: object=%p (%d!=%d)!\n",
                __FILE__, __LINE__, file, line, object, object->sn, sn);
            ++errors;
            break;
        }
        node = chain->peekPrevious(node);
        object = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    if (sn != -1) {
        errorf("%s[%d]: %s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, file, line, sn, -1);
        ++errors;
    }

    if (0 != node) {
        errorf("%s[%d]: %s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, file, line, node, 0);
        ++errors;
    }

    // Done.

    if (errors > 0) {
        errorf("%s[%d]: %s[%d]: failed errors=%d!\n",
                __FILE__, __LINE__, file, line, errors);
        UT_Chain_Dump dump;
        chain->apply(dump);
    }

    return errors;
}

static int rooting(Link* origin, Link* root) {
    Print errorf(Platform::instance().error());
    int errors = 0;
    Link* link;
    int linked;
    int rooted;

    for (int sn = 0; sn < LIMIT; ++sn) {
        if (links[sn]->node.getRoot() != root) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, links[sn]->node.getRoot(), root);
            ++errors;
        }
    }

    linked = 0;
    rooted = 0;
    link = origin;
    do {
        if (link->getRoot() != root) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, link->getRoot(), root);
            ++errors;
        }
        if (link == root) {
            ++rooted;
        }
        ++linked;
        link = link->getNext();
    } while (link != origin);
    if (linked != (LIMIT + 1)) {
        errorf("%s[%d]: (%d!=%d)!\n",
             __FILE__, __LINE__, linked, LIMIT + 1);
         ++errors;
    }
    if (rooted != 1) {
        errorf("%s[%d]: (%d!=%d)!\n",
             __FILE__, __LINE__, rooted, 1);
         ++errors;
    }

    linked = 0;
    rooted = 0;
    link = origin;
    do {
        if (link->getRoot() != root) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, link->getRoot(), root);
            ++errors;
        }
        if (link == root) {
            ++rooted;
        }
        ++linked;
        link = link->getPrevious();
    } while (link != origin);
    if (linked != (LIMIT + 1)) {
        errorf("%s[%d]: (%d!=%d)!\n",
             __FILE__, __LINE__, linked, LIMIT + 1);
         ++errors;
    }
    if (rooted != 1) {
        errorf("%s[%d]: (%d!=%d)!\n",
             __FILE__, __LINE__, rooted, 1);
         ++errors;
    }

    return errors;
}

static Chain staticChain;

CXXCAPI int unittestChain(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;
    Link* node;
    Link* result;
    Link* root;
    UT_Chain_Link* link;
    void* payload;
    int sn;
    bool success;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticChain.show();

    printf("%s[%d]: initialization\n", __FILE__, __LINE__);

    chain = new Chain;
    node = chain;

    UT_Chain_Dump dump;
    result = chain->apply(dump);
    if (0 != result) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, 0, result);
        ++errors;
    }

    if (!(success = chain->isEmpty())) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, success, true);
        ++errors;
    }

    printf("chain=%p\n", chain);

    printf("%s[%d]: show empty\n", __FILE__, __LINE__);

    chain->show(1);

    printf("%s[%d]: insertFirst backward\n",
        __FILE__, __LINE__);

    for (sn = LIMIT - 1; 0 <= sn; --sn) {
        link = new UT_Chain_Link(sn);
        links[sn] = link;
        node = &(link->node);
        printf("links[%d]=%p node=%p\n", sn, links[sn], node);
        if (!(success = node->isRoot())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        if ((success = chain->isMember(node))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        result = chain->insertFirst(node);
        if (result != node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, result, node);
            ++errors;
        }
        if ((success = node->isRoot())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        if (!(success = chain->isMember(node))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        if ((success = chain->isEmpty())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: test rerooting\n", __FILE__, __LINE__);

    Link* urroot = links[0]->node.getRoot();
    for (int ii = 0; LIMIT > ii; ++ii) {
        errors += rooting(&(links[ii]->node), urroot);
    }

    for (int ii = 0; LIMIT > ii; ++ii) {
        Link* result = links[ii]->node.reroot();
        if (result != &(links[ii]->node)) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, result, &(links[ii]->node));
            ++errors;
        }
        for (int jj = 0; LIMIT > jj; ++jj) {
            errors += rooting(&(links[jj]->node), &(links[ii]->node));
        }
    }

    result = urroot->reroot();
    if (result != urroot) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, result, urroot);
        ++errors;
    }
    for (int ii = 0; LIMIT > ii; ++ii) {
        errors += rooting(&(links[ii]->node), urroot);
    }

    printf("%s[%d]: test functor sanity\n", __FILE__, __LINE__);

    UT_Chain_Functor functorOne(ONE);
    result = chain->apply(functorOne);
    if (chain != result) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, chain, result);
        ++errors;
    }

    UT_Chain_Functor functorForward(FORWARD);
    result = chain->apply(functorForward);
    if (0 != result) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, 0, result);
        ++errors;
    }

    UT_Chain_Functor functorReverse(REVERSE);
    result = chain->apply(functorReverse);
    if (0 != result) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, 0, result);
        ++errors;
    }

    printf("%s[%d]: apply one forwards\n", __FILE__, __LINE__);

    UT_Chain_Dump dump1(false, true);
    result = chain->apply(dump1);
    if (chain != result) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, chain, result);
        ++errors;
    }

    printf("%s[%d]: apply one backwards\n", __FILE__, __LINE__);

    UT_Chain_Dump pmud1(false, false);
    result = chain->apply(pmud1);
    if (chain != result) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, chain, result);
        ++errors;
    }

    printf("%s[%d]: apply all forwards\n", __FILE__, __LINE__);

    dump.reset();
    result = chain->apply(dump);
    if (0 != result) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, 0, result);
        ++errors;
    }

    printf("%s[%d]: apply all backwards\n", __FILE__, __LINE__);

    UT_Chain_Dump pmud(true, false);
    result = chain->apply(pmud);
    if (0 != result) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, 0, result);
        ++errors;
    }

    printf("%s[%d]: set/get payload\n", __FILE__, __LINE__);

    node = chain->peekFirst();
    link = (0 == node) ? 0 : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = 0; sn < LIMIT; ++sn) {
        if (node != &(link->node)) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, node, &(link->node));
            ++errors;
            break;
        } else if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        }
        node->setPayload(0);
        node = chain->peekNext(node);
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    node = chain->peekFirst();
    link = (0 == node) ? 0 : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = 0; sn < LIMIT; ++sn) {
        if (0 != link) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, 0);
            ++errors;
            break;
        }
        node->setPayload(links[sn]);
        node = chain->peekNext(node);
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    node = chain->peekFirst();
    link = (0 == node)
                ? 0
                : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = 0; sn < LIMIT; ++sn) {
        if (node != &(link->node)) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, node, &(link->node));
            ++errors;
            break;
        } else if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        }
        node = chain->peekNext(node);
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: removeFirst forward\n", __FILE__, __LINE__);

    for (sn = 0; sn < LIMIT; ++sn) {
        if (0 == (node = chain->removeFirst())) {
            break;
        } else if (0 == (link =
                            static_cast<UT_Chain_Link*>(node->getPayload()))) {
            break;
        } else if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        } else if (link->sn != sn) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, sn);
            ++errors;
            break;
        }
    }

    if (sn != LIMIT) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, sn, LIMIT);
        ++errors;
    }

    node = chain->removeFirst();
    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    printf("%s[%d]: insertLast forward\n", __FILE__, __LINE__);

    for (sn = 0; sn < LIMIT; ++sn) {
        link = links[sn];
        node = &(link->node);
        result = chain->insertLast(node);
        if (result != node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, result, node);
            ++errors;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: removeLast backward\n", __FILE__, __LINE__);

    for (sn = LIMIT - 1; sn >= 0; --sn) {
        if (0 == (node = chain->removeLast())) {
            break;
        } else if (0 == (link =
                            static_cast<UT_Chain_Link*>(node->getPayload()))) {
            break;
        } else if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        } else if (link->sn != sn) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, sn);
            ++errors;
            break;
        }
    }

    if (sn != -1) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, sn, -1);
        ++errors;
    }

    node = chain->removeLast();
    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    printf("%s[%d]: insertLast forward\n", __FILE__, __LINE__);

    link = links[0];
    node = &(link->node);
    result = chain->insertLast(node);
    if (result != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, result, node);
        ++errors;
    }

    for (sn = 1; sn < LIMIT; ++sn) {
        node = &(links[sn]->node);
        result = node->insert(result);
        if (result != node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__,
                result, node);
            ++errors;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: remove odd\n", __FILE__, __LINE__);

    for (sn = 1; sn < LIMIT; sn += 2) {
        if ((success = links[sn]->node.hasRoot(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        if (!(success = chain->isMember(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        links[sn]->node.remove();
        if (!(success = links[sn]->node.hasRoot(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        if ((success = chain->isMember(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
    }

    printf("%s[%d]: chain connectivity\n", __FILE__, __LINE__);

    node = chain->peekFirst();
    link = (0 == node) ? 0 : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = 0; sn < LIMIT; sn += 2) {
        if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        } else if (link->sn != sn) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, sn);
            ++errors;
            break;
        }
        node = chain->peekNext(node);
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    node = chain->peekLast();
    link = (0 == node) ? 0 : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = LIMIT - 2; sn >= 0; sn -= 2) {
        if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        } else if (link->sn != sn) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, sn);
            ++errors;
            break;
        }
        node = chain->peekPrevious(node);
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    printf("%s[%d]: link connectivity\n", __FILE__, __LINE__);

    node = chain->getNext();
    link = (0 == node) ? 0 : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = 0; sn < LIMIT; sn += 2) {
        if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        } else if (link->sn != sn) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, sn);
            ++errors;
            break;
        }
        node = node->getNext();
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    if (chain != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, chain, node);
        ++errors;
    }

    node = chain->getPrevious();
    link = (0 == node) ? 0 : static_cast<UT_Chain_Link*>(node->getPayload());
    for (sn = LIMIT - 2; sn >= 0; sn -= 2) {
        if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        } else if (link->sn != sn) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, sn);
            ++errors;
            break;
        }
        node = node->getPrevious();
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
    }

    if (chain != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, chain, node);
        ++errors;
    }

    printf("%s[%d]: validity\n", __FILE__, __LINE__);

    for (sn = 0; sn < LIMIT; sn += 2) {
        if (!(success = links[sn]->node.isChained())) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, links[sn], success, true);
            ++errors;
        }
        if ((success = links[sn]->node.hasRoot(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        if (!(success = chain->isMember(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        node = links[sn]->node.insert(&(links[sn+1]->node));
        if (0 != node) {
            errorf("%s[%d]: link=%p (%p!=%p)!\n",
                __FILE__, __LINE__, links[sn], node, 0);
        }
        node = chain->insertFirst(&(links[sn]->node));
        if (0 != node) {
            errorf("%s[%d]: link=%p (%p!=%p)!\n",
                __FILE__, __LINE__, links[sn], node, 0);
            ++errors;
        }
        node = chain->insertLast(&(links[sn]->node));
        if (0 != node) {
            errorf("%s[%d]: link=%p (%p!=%p)!\n",
                __FILE__, __LINE__, links[sn], node, 0);
            ++errors;
        }
        node = chain->insertNext(&(links[sn]->node), &(links[sn]->node));
        if (0 != node) {
            errorf("%s[%d]: link=%p (%p!=%p)!\n",
                __FILE__, __LINE__, links[sn], node, 0);
            ++errors;
        }
        node = chain->insertPrevious(&(links[sn]->node), &(links[sn]->node));
        if (0 != node) {
            errorf("%s[%d]: link=%p (%p!=%p)!\n",
                __FILE__, __LINE__, links[sn], node, 0);
            ++errors;
        }
    }

    for (sn = 1; sn < LIMIT; sn += 2) {
        if ((success = links[sn]->node.isChained())) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, links[sn], success, false);
            ++errors;
        }
        if (!(success = links[sn]->node.hasRoot(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        if ((success = chain->isMember(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        node = links[sn]->node.remove();
        if (0 != node) {
            errorf("%s[%d]: link=%p (%p!=%p)!\n",
                __FILE__, __LINE__, links[sn], node, 0);
        }
        node = chain->insertNext(&(links[sn]->node), &(links[sn]->node));
        if (0 != node) {
            errorf("%s[%d]: link=%p (%p!=%p)!\n",
                __FILE__, __LINE__, links[sn], node, 0);
            ++errors;
        }
        node = chain->insertPrevious(&(links[sn]->node), &(links[sn]->node));
        if (0 != node) {
            errorf("%s[%d]: link=%p (%p!=%p)!\n",
                __FILE__, __LINE__, links[sn], node, 0);
            ++errors;
        }
    }

    printf("%s[%d]: insert odd\n", __FILE__, __LINE__);

    for (sn = 1; sn < LIMIT; sn += 2) {
        if (!(success = links[sn]->node.hasRoot(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        if ((success = chain->isMember(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        node = links[sn]->node.insert(&(links[sn - 1]->node));
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
        if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
        }
        if ((success = links[sn]->node.hasRoot(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        if (!(success = chain->isMember(&(links[sn]->node)))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: removeNext\n", __FILE__, __LINE__);

    for (sn = 0; sn < LIMIT; ++sn) {
        node = chain->removeNext(chain);
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
        if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        } else if (link->sn != sn) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, sn);
            ++errors;
            break;
        }
    }

    if (sn != LIMIT) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, sn, LIMIT);
        ++errors;
    }

    node = chain->removeNext(chain);
    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    if (!(success = chain->isEmpty())) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, success, true);
        ++errors;
    }

    printf("%s[%d]: insertPrevious\n", __FILE__, __LINE__);

    for (sn = 0; sn < LIMIT; ++sn) {
        link = links[sn];
        node = &(link->node);
        result = chain->insertPrevious(chain, node);
        if (result != node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, result, node);
            ++errors;
        }
        if ((success = chain->isEmpty())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: removePrevious\n", __FILE__, __LINE__);

    for (sn = LIMIT - 1; sn >= 0; --sn) {
        node = chain->removePrevious(chain);
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
        if (link != links[sn]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn]);
            ++errors;
            break;
        } else if (link->sn != sn) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, sn);
            ++errors;
            break;
        }
    }

    if (sn != -1) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, sn, LIMIT);
        ++errors;
    }

    node = chain->removePrevious(chain);
    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    printf("%s[%d]: insertNext\n", __FILE__, __LINE__);

    for (sn = LIMIT - 1; sn >= 0; --sn) {
        link = links[sn];
        node = &(link->node);
        result = chain->insertNext(chain, node);
        if (result != node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, result, node);
            ++errors;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: removeNext odd\n", __FILE__, __LINE__);

    for (sn = 0; sn < LIMIT; sn += 2) {
        node = chain->removeNext(&(links[sn]->node));
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
        if (link != links[sn + 1]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn + 1]);
            ++errors;
            break;
        } else if (link->sn != (sn + 1)) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, (sn + 1));
            ++errors;
            break;
        }
    }

    printf("%s[%d]: insertNext odd\n", __FILE__, __LINE__);

    for (sn = 0; sn < LIMIT; sn += 2) {
        node = chain->insertNext(&(links[sn]->node), &(links[sn+1]->node));
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
        if (link != links[sn + 1]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn + 1]);
            ++errors;
            break;
        } else if (link->sn != (sn + 1)) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, (sn + 1));
            ++errors;
            break;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: removePrevious even\n", __FILE__, __LINE__);

    for (sn = 1; sn < LIMIT; sn += 2) {
        node = chain->removePrevious(&(links[sn]->node));
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
        if (link != links[sn - 1]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn - 1]);
            ++errors;
            break;
        } else if (link->sn != (sn - 1)) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, (sn - 1));
            ++errors;
            break;
        }
    }

    printf("%s[%d]: insertPrevious even\n", __FILE__, __LINE__);

    for (sn = 1; sn < LIMIT; sn += 2) {
        node = chain->insertPrevious(&(links[sn]->node), &(links[sn-1]->node));
        link = (0 == node)
                    ? 0
                    : static_cast<UT_Chain_Link*>(node->getPayload());
        if (link != links[sn - 1]) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, link, links[sn - 1]);
            ++errors;
            break;
        } else if (link->sn != (sn - 1)) {
            errorf("%s[%d]: link=%p (%d!=%d)!\n",
                __FILE__, __LINE__, link, link->sn, (sn - 1));
            ++errors;
            break;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    printf("%s[%d]: apply first\n", __FILE__, __LINE__);

    UT_Chain_Dump first(false);
    result = chain->apply(first);
    if (result != chain) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, result, chain);
        ++errors;
    }

    printf("%s[%d]: apply forwards\n", __FILE__, __LINE__);

    UT_Chain_Dump forwards;
    result = chain->apply(forwards);
    if (result != 0) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, result, 0);
        ++errors;
    }

    printf("%s[%d]: apply backwards\n", __FILE__, __LINE__);

    UT_Chain_Dump backwards(true, false);
    result = chain->apply(backwards);
    if (result != 0) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, result, 0);
        ++errors;
    }

    printf("%s[%d]: apply remove forwards\n",
        __FILE__, __LINE__);

    UT_Chain_Remove remove;
    result = chain->apply(remove);
    if (result != 0) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, result, 0);
        ++errors;
    }
    if (!(success = chain->isEmpty())) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, success, true);
        ++errors;
    }

    chain->apply(dump);

    printf("%s[%d]: apply insert forwards\n",
        __FILE__, __LINE__);

    UT_Chain_Insert insert;
    result = chain->apply(insert);
    if (result == 0) {
        errorf("%s[%d]: (0x%x==0x%x)!\n",
            __FILE__, __LINE__, result, 0);
        ++errors;
    }

    errors += audit(chain, __FILE__, __LINE__);

    chain->apply(dump);

    printf("%s[%d]: apply remove backwards\n",
        __FILE__, __LINE__);

    UT_Chain_Remove evomer(false);
    result = chain->apply(evomer);
    if (result != 0) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, result, 0);
        ++errors;
    }
    if (!(success = chain->isEmpty())) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, success, true);
        ++errors;
    }

    chain->apply(dump);

    printf("%s[%d]: apply insert backwards\n",
        __FILE__, __LINE__);

    UT_Chain_Insert tresni(false);
    result = chain->apply(tresni);
    if (result == 0) {
        errorf("%s[%d]: (0x%x==0x%x)!\n",
            __FILE__, __LINE__, result, 0);
        ++errors;
    }

    errors += audit(chain, __FILE__, __LINE__);

    chain->apply(dump);

    printf("%s[%d]: show full\n", __FILE__, __LINE__);

    chain->show(1);

    printf("%s[%d]: link destruction\n", __FILE__, __LINE__);

    for (sn = 0; sn < LIMIT; ++sn) {
        if ((success = chain->isEmpty())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        link = links[sn];
        node = &(link->node);
        if (!(success = chain->isMember(node))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        delete link;
    }

    if (!(success = chain->isEmpty())) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, success, true);
        ++errors;
    }

    node = chain->peekFirst();
    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    node = chain->peekLast();
    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    node = chain->removeFirst();
    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    node = chain->removeLast();
    if (0 != node) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, 0);
        ++errors;
    }

    printf("%s[%d]: root\n", __FILE__, __LINE__);

    root = chain;
    payload = root->getPayload();
    if (0 != payload) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, 0, payload);
        ++errors;
    }
    node = root->getRoot();
    if (node != root) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, node, root);
        ++errors;
    }

    chain->apply(dump);

    printf("%s[%d]: chain destruction\n", __FILE__, __LINE__);

    root = chain;
    for (sn = LIMIT - 1; sn >= 0; --sn) {
        link = new UT_Chain_Link(sn);
        links[sn] = link;
        node = &(link->node);
        if (!(success = node->isRoot())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        if ((success = chain->isMember(node))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        result = node->insert(root);
        if (result != node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, result, node);
            ++errors;
        }
        if ((success = node->isRoot())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__,
                success, false);
            ++errors;
        }
        if (!(success = chain->isMember(node))) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        if ((success = chain->isEmpty())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
    }

    errors += audit(chain, __FILE__, __LINE__);

    delete chain;

    for (sn = 0; sn < LIMIT; ++sn) {
        link = links[sn];
        node = &(link->node);
        if ((success = node->isChained())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, false);
            ++errors;
        }
        if (!(success = node->isRoot())) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, success, true);
            ++errors;
        }
        root = node->getRoot();
        if (root != node) {
            errorf("%s[%d]: (%p!=%p)!\n",
                __FILE__, __LINE__, root, node);
            ++errors;
        }
        chain->apply(dump);
        delete link;
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
