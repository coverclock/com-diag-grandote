#ifndef _COM_DIAG_DESPERADO_LINK_H_
#define _COM_DIAG_DESPERADO_LINK_H_

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
 *  Declares the Link class.
 *
 *  @see    Link
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an object which can be inserted into and removed from a
 *  circular doubly-linked list, or chain, in which insertions and
 *  removals can be done in O(1) time, the head of the chain can be
 *  accessed in O(1) time, and a payload object can be on multiple chains
 *  simultaneously.
 *
 *  Each chain link has a pointer to the next link on the chain, and
 *  another pointer to the previous chain link on the chain. For a
 *  chain link which is not on a chain, both the next and previous
 *  pointers of the chain link point to the chain link object itself.
 *  Hence, a single chain link all by itself constitutes a circular
 *  doubly-linked list. This is true whether the chain link is newly
 *  constructed, has never been inserted onto a chain, or has been
 *  removed from a chain.
 *
 *  The circularity is a simplification, not a complication, as
 *  it eliminates all the special cases present with null-terminated
 *  doubly-linked lists. The next and previous pointers allow any chain
 *  link to be inserted into or removed from any arbitrary position in
 *  the chain in O(1) time.
 *
 *  Each chain link also has a pointer to its root. When a chain link
 *  is inserted onto a chain, it is always inserted after an existing
 *  chain link that is already on the chain. When this is done, the
 *  inserted chain link inherits the root pointer of the chain link
 *  that is already on the chain. For a chain link which is not on a
 *  chain, the root pointer of the chain link points to the chain link
 *  object itself. This is true whether the chain link is newly constructed,
 *  has never been inserted onto a chain, or has been removed from a chain.
 *
 *  Using a chain link as the head or root of the chain, as additional
 *  chain links are inserted onto the chain, the initial root pointer
 *  is propagated into each inserted chain link. The root can be
 *  accessed from any chain link on the chain. The root pointer allows
 *  the chain membership of any arbirary chain link to be determined in O(1)
 *  time.
 *
 *  Accessing the next, previous or root pointers of a chain link
 *  always returns a valid pointer to a chain link (possibly the same
 *  chain link). There is never a null moment with the next, previous
 *  or root pointers of a chain link.
 *
 *  Each chain link also has a payload pointer. The payload pointer is a
 *  void* pointer. It is the only pointer in a chain link
 *  that is initialized to null when a chain link is constructed. The
 *  payload pointer can be accessed and set by the application.
 *  No other chain link operation references the payload pointer.
 *
 *  A chain link can exist independently of the payload object to which
 *  its payload pointer points (as the STL container classes do). But a
 *  chain link can also exist as an instance variable inside the payload
 *  object. This makes memory management of chain links much simpler, and
 *  also couples the destruction of the chain link to the destruction of the
 *  payload object. Furthermore, a payload object may contain more than
 *  one chain link instance variable, allowing it to be on more than one
 *  chain at a time. This class can also be used via inheritance rather
 *  than by composition, but this is strongly discouraged.
 *
 *  Upon destruction, a chain link is removed from any chain it may
 *  be on. This prevents any dangling pointers from hanging around
 *  inside chain links on a chain. The exception to this is the
 *  destruction of the chain link that is the root of a chain.
 *  Destroying a chain link that is the root of a chain will leave
 *  an intact chain in which each chain link has valid next and
 *  previous pointers, but with a root pointer pointing to the
 *  destroyed chain link. (The companion class Chain
 *  addresses this issue by making the root of a chain a special
 *  case of a chain link, in which this clean up is performed
 *  automatically when the root of the chain is destroyed. The
 *  cleanup requires that the entire chain be traversed in O(n)
 *  time. Destroying the chain object leaves all of the former
 *  chain links in their newly constructed states with the exception
 *  of their intact payload pointers.)
 *
 *  It is an error to try to insert a chain link that is already on a
 *  chain. It is an error to try to remove a chain link that is not on
 *  a chain. The caller should check the return code on insert and remove
 *  operations to detect these errors.
 *
 *  The pointers in a chain link are declared to be volatile
 *  so that for applications using chains in shared memory and synchronizing
 *  updates with a suitable platform-specific mechanism, the compiler commits
 *  pointer updates to memory as quickly as possible.
 *
 *  This class contains no virtual functions so that a virtual table pointer
 *  is not allocated as part of the object. This doesn't sound so important,
 *  but in an application in which thousands of linked objects may be
 *  allocated, a single four-byte pointer makes a difference. Links already
 *  have enough overhead that for some applications their use will be
 *  problematic.
 *
 *  This class is not thread-safe. Serialization is the responsibility
 *  of the caller. Adding a thread safe wrapper using the appropriate
 *  native synchronization mechanism of the underlying platform (and as
 *  an aside, giving the payload pointer a specific type definition), seems
 *  like a good idea.
 *
 *  Thanks to the author(s) of the Linux kernel in which the doubly-linked
 *  list code inspired this design.
 *
 *  @see    T. Aivanzian, <I>Linux Kernel 2.4 Internals</I>,
 *          August 2001, p. 19-21
 *
 *  @see    "Linked list", Wikipedia, http://en.wikipedia.org/wiki/Linked_list
 *
 *  @see    Chain
 *
 *  @author coverclock@diag.com (Chip Overclock),
 *          T. Aivanzian
 */
class Link {

public:

    /**
     *  Constructor. A newly constructed chain link is chained
     *  to itself.
     *
     *  @param  object      points to a value to be stored in the payload
     *                      of this link.
     */
    explicit Link(void* object = 0);

    /**
     *  Destructor. If this link is on a chain, it is removed from that
     *  chain.
     */
    ~Link();

    /**
     *  Gets the pointer to the link on the chain after this link.
     *  If this link is on a chain, then following the next pointers
     *  will eventually expose the address of the chain root. This can
     *  be done trivially by getting the first link on a chain then
     *  getting that link's previous pointer. This works even if the
     *  chain is empty. Hence, it is easy to derive the chain root pointer
     *  and compare subsequent pointers to it to determine if you have
     *  reached the end of the chain. If the returned pointer is equal
     *  to this, then this link is not on a chain.
     *
     *  @return a pointer to the next link.
     */
    Link* getNext() const;

    /**
     *  Gets the pointer to the link on the chain before this link.
     *  If this link is on a chain, then following the previous pointers
     *  will eventually expose the address of the chain root. This can
     *  be done trivially by getting the last link on a chain then
     *  getting that links next pointer. This works even if the
     *  chain is empty. Hence, it is easy to derive the chain root pointer
     *  and compare subsequent pointers to it to determine if you have
     *  reached the end of the chain. If the returned pointer is equal
     *  to this, then this link is not on a chain.
     *
     *  @return a pointer to the previous link.
     */
    Link* getPrevious() const;

    /**
     *  Gets the pointer to the root of the chain that this link is on.
     *  If the link is not on a chain with another link, this this pointer
     *  points to this link and this link constitutes a chain of one.
     *
     *  @return a pointer to the previous link.
     */
    Link* getRoot() const;

    /**
     *  Gets the void pointer in the payload of this link.
     *
     *  @return the void pointer stored in the payload of this link.
     */
    void* getPayload() const;

    /**
     *  Stores a void pointer in the payload of this link.
     *
     *  @param  object      points to a value to be stored in the payload
     *                      of this link.
     *
     *  @return the void pointer now stored in the payload of this link.
     */
    void* setPayload(void* object);

    /**
     *  Returns true if this link in on a chain. Links that are chain
     *  roots but are empty are identical to links that are not on
     *  a chain.
     *
     *  @return true if this link is on a chain.
     */
    bool isChained() const;

    /**
     *  Returns true if this link has that link as its root.
     *
     *  @return true if this link has that link as its root.
     */
    bool hasRoot(const Link* that) const;

    /**
     *  Returns true if this link is a root.
     *
     *  @return true if this link is a root.
     */
    bool isRoot() const;

    /**
     *  If this link is in an chain, removes it from the chain.
     *
     *  @return a pointer to this link, or null (0) if this link
     *          is not on a chain.
     */
    Link* remove();

    /**
     *  If this link is not already on a chain, inserts it onto a chain
     *  just after that link. This link inherits the root of that link.
     *
     *  @param  that    is the link that this link is inserted afterwards.
     *
     *  @return a pointer to this link, or null (0) if this link is not
     *          on a chain.
     */
    Link* insert(Link* that);

    /**
     *  Change all of the links on the chain that this link is on
     *  to be rooted to this link. This is useful when you remove the
     *  root of a chain and have to re-root the chain to a new link.
     *
     *  @return a pointer to this link.
     */
    Link* reroot();

    /**
     *  Defines an interface to a functor (an object that can be called
     *  like a function), that performs an operation on a link.
     */
    class Functor {

    public:

        /**
         *  Dtor.
         */
        virtual ~Functor();

        /**
         *  Perform an application-defined operation on a link. The
         *  functor returns a pointer to the next link to which it
         *  is to be applied, or null (zero) if finished.
         *
         *  @param  that        points to a link object.
         *
         *  @return a pointer to a link or null (zero).
         */
        virtual Link* operator() (Link* that) = 0;

    };

    /**
     *  Applies a functor to links in a chain, starting with this
     *  link. If the functor returns a pointer to a link that is not
     *  the link whose pointer was passed to it, the functor is then
     *  applied to that link whose pointer it returned. If the functor
     *  returns the same pointer that was passed to it, this method
     *  returns that pointer. If the functor returns null, this method
     *  returns null.
     *
     *  @param  functor         refers to the functor to be applied.
     *
     *  @return the last value returns by the functor: a pointer to a
     *  link or null.
     */
    Link* apply(Functor& functor);

    /**
     *  Displays internal information about this object to the specified
     *  output object. Useful for debugging and troubleshooting.
     *
     *  @param  level   sets the verbosity of the output. What this means
     *                  is object dependent. However, the level is passed
     *                  from outer to inner objects this object calls the
     *                  show methods of its inherited or composited objects.
     *
     *  @param display  points to the output object to which output is
     *                  sent. If null (zero), the default platform output
     *                  object is used as the effective output object. The
     *                  effective output object is passed from outer to
     *                  inner objects as this object calls the show methods
     *                  of its inherited and composited objects.
     *
     *  @param  indent  specifies the level of indentation. One more than
     *                  this value is passed from outer to inner objects
     *                  as this object calls the show methods of its
     *                  inherited and composited objects.
     */
    void show(int level = 0, Output* display = 0, int indent = 0) const;

private:

    /**
     *  Copy constructor.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Link(const Link& that);

    /**
     *  Assignment operator.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Link& operator=(const Link& that);

    /**
     *  Points to the next link after this link on a chain. If this
     *  link is not on a chain, this pointer points to this link, which
     *  is its value upon construction.
     */
    Link*volatile next;

    /**
     *  Points to the previous link before this link on a chain. If
     *  this link is not on a chain, this pointer points to this link,
     *  which is its value upon construction.
     */
    Link*volatile previous;

    /**
     *  Points to the chain link which serves as the chain root for the
     *  chain on which which this link is chained. If this link is not
     *  on a chain, this pointer points to this link, which is its
     *  value upon construction.
     */
    Link*volatile root;

    /**
     *  Points to the payload of this link. Its value upon construction
     *  is null (0).
     */
    void*volatile payload;

};


//
//  Constructor. Upon construction, this chain link is chained to
//  itself.
//
inline Link::Link(void* object) :
    next(this),
    previous(this),
    root(this),
    payload(object)
{
}


//
//  Return a pointer to the link on the chain after this link.
//
inline Link* Link::getNext() const {
    return this->next;
}


//
//  Return a pointer to the object on the chain before this link.
//
inline Link* Link::getPrevious() const {
    return this->previous;
}


//
//  Return a pointer to the link that is the root of this link.
//
inline Link* Link::getRoot() const {
    return this->root;
}


//
//  Return a pointer to the payload on this link.
//
inline void* Link::getPayload() const {
    return this->payload;
}


//
//  Set the payload of this link to the specified pointer.
//
inline void* Link::setPayload(void* object) {
    return this->payload = object;
}


//
//  Return true if this link is on a chain.
//
inline bool Link::isChained() const {
    return (this != this->next) ||
           (this != this->previous);
}


//
//  Return true if this link has that link as its root.
//
inline bool Link::hasRoot(const Link* that) const {
    return this->root == that;
}


//
//  Return true if this link is a root.
//
inline bool Link::isRoot() const {
    return this->hasRoot(this);
}



//
//  Remove this link from a chain.
//
inline Link* Link::remove() {
    if (this->isChained()) {
        this->next->previous = this->previous;
        this->previous->next = this->next;
        this->next = this->previous = this->root = this;
        return this;
    }
    return 0;
}


//
//  Insert this link onto a chain just after that link. This
//  link inherits the root of that link.
//
inline Link* Link::insert(Link* that) {
    if (!this->isChained()) {
        this->next = that->next;
        this->previous = that;
        this->root = that->root;
        that->next->previous = that->next = this;
        return this;
    }
    return 0;
}


//
//  Destructor. Upon destruction, this chain link is removed from
//  any chain of which is may be a member.
//
inline Link::~Link() {
    this->remove();
}

#include "com/diag/desperado/End.h"


#endif
