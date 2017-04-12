#ifndef _COM_DIAG_DESPERADO_CHAIN_H_
#define _COM_DIAG_DESPERADO_CHAIN_H_

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
 *  Declares the Chain class.
 *
 *  @see    Chain
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Link.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a circular doubly-linked list of Link objects which
 *  can be used to implement traditional non-circular data structures
 *  such as queues or stacks. A chain itself is a special case of a
 *  link object.
 *
 *  The basic operations are put a chain link on the chain (insert),
 *  take a chain link off the chain (remove), and access a chain link
 *  that is on the chain without modifying the chain (peek). Each operation
 *  may be relative to the head of the chain (First), to the tail of the
 *  chain (Last), to the position just after a link on the chain (Next),
 *  or to the position just before a link on the chain (Previous).
 *
 *  The chain provides the illusion of non-circularity by recognizing
 *  when operations on the chain have wrapped around to the root of the
 *  chain and returning a null (0) pointer as a result.
 *
 *  Since a chain object is merely a special case of a chain link, it may
 *  contain its own void pointer to a payload. This allows an application to,
 *  for example, store a pointer to the owning object inside the chain object.
 *  Hence, the owning object may be accessed from any link on the chain
 *  by acessing that link's root and from there the root's payload.
 *
 *  It is an error to try to insert, remove, or peek a chain link next or
 *  previous to a chain link that is not on this chain. It is an error to
 *  try to insert a chain link that is already on a chain. The caller
 *  should check the return code on insert, remove and peek operations to
 *  detect these errors.
 *
 *  This class is not thread-safe. Serialization is the responsibility
 *  of the caller. Adding a thread safe wrapper using the appropriate
 *  native synchronization mechanism of the underlying platform (and as
 *  an aside, giving the payload pointer a specific type definition), seems
 *  like a good idea.
 *
 *  @see    "Linked list", Wikipedia, http://en.wikipedia.org/wiki/Linked_list
 *
 *  @see    Link
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Chain : public Link {

public:

    /**
     *  Constructor. This chain will be empty and its payload
     *  pointer will be null.
     */
    explicit Chain();

    /**
     *  Destructor. If this chain contains links, all of the
     *  links are removed from the chain. This leaves this
     *  chain object and all of its former link objects in
     *  their newly constructed states with the exception of
     *  their unchanged payload pointers.
     */
    virtual ~Chain();

    /**
     *  Return true if this chain is empty.
     *
     *  @return true if this chain is empty.
     */
    bool isEmpty() const;

    /**
     *  Return true if the specified link has this chain as its root.
     *
     *  @param  link        points to the chain link to be checked.
     *
     *  @return true if the link is on this chain, false otherwise.
     */
    bool isMember(const Link* link) const;

    /**
     *  Examines the first chain link on this chain and returns
     *  a pointer to it. If the chain is empty, returns null (0).
     *
     *  @return a pointer to the first chain link on this chain,
     *          or null (0) if there is no first chain link.
     */
    virtual Link* peekFirst() const;

    /**
     *  Examines the last chain link on this chain and returns
     *  a pointer to it. If the chain is empty, returns null (0).
     *
     *  @return a pointer to the last chain link on this chain,
     *          or null (0) if there is no last chain link.
     */
    virtual Link* peekLast() const;

    /**
     *  Given a chain link, returns a pointer to the next chain
     *  link on a chain. Preserves the illusion of a non-circular
     *  linked list by returning null (0) if there is no next (or
     *  first) link, or if the previous link does not have this
     *  chain as its root.
     *
     *  @param  previous    points to a chain link on this chain.
     *
     *  @return a pointer to the next chain link, or null (0) if
     *          there is no next or first chain link or if the
     *          previous link is not on this chain.
     */
    virtual Link* peekNext(const Link* previous) const;

    /**
     *  Given a chain link, returns a pointer to the previous
     *  chain link on a chain. Preserves the illusion of a non-circular
     *  linked list by returning null (0) if there is no previous (or
     *  last) link, or if the next link does not have this chain
     *  as its root.
     *
     *  @param  next        points to a chain link on this chain.
     *
     *  @return a pointer to the previous chain link, or null (0) if
     *          there is no previous or last chain link or if the
     *          next link is not on this chain.
     */
    virtual Link* peekPrevious(const Link* next) const;

    /**
     *  Removes the first chain link from this chain and returns
     *  a pointer to it. If the chain is empty, returns null (0).
     *
     *  @return a pointer to the first chain link on this chain,
     *          or null (0) if this chain is empty.
     */
    virtual Link* removeFirst();

    /**
     *  Removes the last chain link from this chain and returns
     *  a pointer to it. If the chain is empty, returns null (0).
     *
     *  @return a pointer to the last chain link on this chain,
     *          or null (0) if this chain is empty.
     */
    virtual Link* removeLast();

    /**
     *  Removes the next chain link following the previous chain
     *  link and returns a pointer to it. If there is no
     *  next (or first) link, or if the previous link does not
     *  have this chain as its root, returns null (0).
     *
     *  @param  previous    points to a chain link on this chain.
     *
     *  @return a pointer to the next chain link, or null (0) if there
     *          is no next or first chain link or if the previous
     *          link is not on this chain.
     */
    virtual Link* removeNext(Link* previous);

    /**
     *  Removes the previous chain link before the next chain
     *  link on a chain and returns a pointer to it. If there is no
     *  previous (or last) link, or if the next link does not have
     *  this chain as its root, returns null (0).
     *
     *  @param  next        points to a chain link on this chain.
     *
     *  @return a pointer to the previous chain link, or null (0) if
     *          there is no previous or last chain link, or if the
     *          next link is not on this chain.
     */
    virtual Link* removePrevious(Link* next);

    /**
     *  Inserts a chain link on this chain such that afterwards it is
     *  the first link on the chain. The link inherits this chain
     *  as its root. Returns null (0) if link is already
     *  on a chain.
     *
     *  @param  link        points to a chain link to be inserted onto
     *                      this chain.
     *
     *  @return a pointer to the specified link, or null (0) if the
     *          link is already on a chain.
     */
    virtual Link* insertFirst(Link* link);

    /**
     *  Insert a chain link on this chain such that afterwards
     *  it is the last link on the chain. The link inherits this chain
     *  as its root. Returns null (0) if the link is already
     *  on a chain.
     *
     *  @param  link        points to a chain link to be inserted onto
     *                      this chain.
     *
     *  @return a pointer to the specified link, or null (0) if the
     *          link is already on a chain.
     */
    virtual Link* insertLast(Link* link);

    /**
     *  Insert a chain link on a chain such that afterwards
     *  it follows the specified previous chain link on the chain.
     *  If the link is already on a chain, null (0) is returned and nothing
     *  is changed.
     *
     *  @param  previous    points to a chain link on this chain.
     *
     *  @param  link        points to a chain link to be inserted.
     *                      after the specified previous chain link.
     *
     *  @return a pointer to the inserted link or null (0) if the
     *          link is already on a chain or if the previous link
     *          is not on this chain.
     */
    virtual Link* insertNext(Link* previous, Link* link);

    /**
     *  Insert a chain link on a chain such that afterwards
     *  it preceeds the specified next chain link on the chain.
     *  If the link is already on a chain, null (0) is returned and nothing
     *  is changed.
     *
     *  @param  next        points to a chain link on this chain.
     *
     *  @param  link        points to a chain link to be inserted
     *                      before the specified next chain link.
     *
     *  @return a pointer to the link or null (0) if the link is
     *          already on a chain or if the next link is not on
     *          this chain.
     */
    virtual Link* insertPrevious(Link* next, Link* link);

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
    virtual void show(int level = 0, Output* display = 0, int indent = 0) const;

private:

    /**
     *  Copy constructor.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Chain(const Chain& that);

    /**
     *  Assignment operator.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Chain& operator=(const Chain& that);

};


//
//  Return true if the link has this chain as its root.
//
inline bool Chain::isMember(const Link* link) const {
    return link->hasRoot(this);
}


//
//  Return true if this chain is empty.
//
inline bool Chain::isEmpty() const {
    return !this->isChained();
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Chain unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestChain(void);
#endif


#endif
