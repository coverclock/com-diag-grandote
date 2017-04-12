/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the Chain class.
 *
 *  @see    Chain
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


//
//  Class Chain Implementation
//
//  @author coverclock@diag.com (Chip Overclock)
//


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Chain.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor. Upon construction, this chain is empty.
//
Chain::Chain() : Link() {
}


//
//  Destructor. Upon destruction, any chain links on this chain
//  are removed, preventing any dangling root pointers to this
//  chain remaining in the links.
//
Chain::~Chain() {
    Link* next = this->getNext();

    while (this != next) {
        next->remove();
        next = this->getNext();
    }

}


//
//  Return a pointer to the first link on this chain, or null (0)
//  if this chain is empty.
//
Link* Chain::peekFirst() const {
    Link* next = this->getNext();

    if (this != next) {
        return next;
    }

    return 0;
}


//
//  Return a pointer to the last link on this chain, or null (0)
//  if this chain is empty.
//
Link* Chain::peekLast() const {
    Link* previous = this->getPrevious();

    if (this != previous) {
        return previous;
    }

    return 0;
}


//
//  Return a pointer to the next link on this chain after the previous
//  link, or null (0) if the previous link is the last link on this chain
//  or if the previous link is not on this chain.
//
Link* Chain::peekNext(const Link* previous) const {
    const Link* root = previous->getRoot();

    if (this == root) {
        Link* next = previous->getNext();
        if (this != next) {
            return next;
        }
    }

    return 0;
}


//
//  Return a pointer to the previous link on this chain before the
//  next link, or null (0) if the next link is the first link on this
//  chain or if the next link is not on this chain.
//
Link* Chain::peekPrevious(const Link* next) const {

    if (this == next->getRoot()) {
        Link* previous = next->getPrevious();
        if (this != previous) {
            return previous;
        }
    }

    return 0;
}


//
//  Remove the first link from this chain and return a pointer to
//  it, or null (0) if this chain is empty.
//
Link* Chain::removeFirst() {
    Link* next = this->getNext();

    if (this != next) {
        return next->remove();
    }

    return 0;
}


//
//  Remove the last link from this chain and return a pointer to
//  it, or null (0) if this chain is empty.
//
Link* Chain::removeLast() {
    Link* previous = this->getPrevious();

    if (this != previous) {
        return previous->remove();
    }

    return 0;
}


//
//  Remove the next link after the specified link from this chain
//  and return a pointer to it, or null (0) if the specified link is
//  the last link on the chain.
//
Link* Chain::removeNext(Link* previous) {

    if (this == previous->getRoot()) {
        Link* next = previous->getNext();
        if (this != next) {
            return next->remove();
        }
    }

    return 0;
}


//
//  Remove the next link before the specified link from this chain
//  and return a pointer to it, or null (0) if the specified link is
//  the last link on this chain.
//
Link* Chain::removePrevious(Link* next) {

    if (this == next->getRoot()) {
        Link* previous = next->getPrevious();
        if (this != previous) {
            return previous->remove();
        }
    }

    return 0;
}


//
//  Insert an link into the first position on this chain.
//  Returns the pointer to the link, or null (0) if the
//  link is already on a chain.
//
Link* Chain::insertFirst(Link* link) {

    if (!link->isChained()) {
        return link->insert(this);
    }

    return 0;
}


//
//  Insert an link into the last position on this chain.
//  Returns the pointer to the link, or null (0) if the
//  link is already on a chain.
//
Link* Chain::insertLast(Link* link) {

    if (!link->isChained()) {
         return link->insert(this->getPrevious());
    }

    return 0;
}


//
//  Insert a link into the next position on this chain after
//  the specified previous link. Returns the pointer to the inserted link
//  or null (0) if the link is already on a chain or if the previous
//  link is not on this chain.
//
Link* Chain::insertNext(Link* previous, Link* link) {

    if (this == previous->getRoot()) {
        return link->insert(previous);
    }

    return 0;
}


//
//  Insert a link into the previous position on this chain before
//  the specified next link. Returns the pointer to the inserted link
//  or null (0) if the link is already on a chain or the next link
//  is not on this chain.
//
Link* Chain::insertPrevious(Link* next, Link* link) {

    if (this == next->getRoot()) {
        return link->insert(next->getPrevious());
    }

    return 0;
}


//
//  Show this object on the output object.
//
void Chain::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Link::show(level, display, indent + 1);
    if (0 < level) {
        Link* next;
        int count = 0;
        for (next = this->getNext(); this != next; next = next->getNext()) {
            printf("%s link[%d]:\n", sp, count++);
            next->show(level, display, indent + 2);
        }
    }
}


#include "com/diag/desperado/End.h"
