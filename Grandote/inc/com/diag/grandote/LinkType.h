#ifndef _COM_DIAG_DESPERADO_LINKTYPE_H_
#define _COM_DIAG_DESPERADO_LINKTYPE_H_

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
 *  Declares the LinkType template.
 *
 *  @see    LinkType
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Link.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Offers a type-safe implemtation of Link by allowing the type
 *  of the link to be based on that of the payload object.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
template <typename _TYPE_>
class LinkType : public Link {

public:

    /**
     *  Constructor. A newly constructed chain link is chained
     *  to itself.
     *
     *  @param  object      points to a value to be stored in the payload
     *                      of this link.
     */
    explicit LinkType(_TYPE_* object = 0);

    /**
     *  Destructor. If this link is on a chain, it is removed from that
     *  chain.
     */
    ~LinkType();

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
    LinkType<_TYPE_>* getNext() const;

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
    LinkType<_TYPE_>* getPrevious() const;

    /**
     *  Gets the pointer to the root of the chain that this link is on.
     *  If the link is not on a chain with another link, this this pointer
     *  points to this link and this link constitutes a chain of one.
     *
     *  @return a pointer to the previous link.
     */
    LinkType<_TYPE_>* getRoot() const;

    /**
     *  Gets the void pointer in the payload of this link.
     *
     *  @return the void pointer stored in the payload of this link.
     */
    _TYPE_* getPayload() const;

    /**
     *  Stores a void pointer in the payload of this link.
     *
     *  @param  object      points to a value to be stored in the payload
     *                      of this link.
     *
     *  @return the void pointer now stored in the payload of this link.
     */
    _TYPE_* setPayload(_TYPE_* object);

    /**
     *  Returns true if this link has that link as its root.
     *
     *  @return true if this link has that link as its root.
     */
    bool hasRoot(const LinkType<_TYPE_>* that) const;

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
    LinkType<_TYPE_>* remove();

    /**
     *  If this link is not already on a chain, inserts it onto a chain
     *  just after that link. This link inherits the root of that link.
     *
     *  @param  that    is the link that this link is inserted afterwards.
     *
     *  @return a pointer to this link, or null (0) if this link is not
     *          on a chain.
     */
    LinkType<_TYPE_>* insert(LinkType<_TYPE_>* that);

    /**
     *  Change all of the links on the chain that this link is on
     *  to be rooted to this link. This is useful when you remove the
     *  root of a chain and have to re-root the chain to a new link.
     *
     *  @return a pointer to this link.
     */
    LinkType<_TYPE_>* reroot();

    /**
     *  Defines an interface to a functor (an object that can
     *  be called like a function), that performs an operation on a link.
     */
    class Functor {

    public:

        /**
         * Dtor.
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
        virtual LinkType<_TYPE_>* operator() (LinkType<_TYPE_>* that) = 0;

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
    LinkType<_TYPE_>*  apply(Functor& functor);

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
    LinkType(const LinkType<_TYPE_>& that);

    /**
     *  Assignment operator.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    LinkType<_TYPE_>& operator=(const LinkType<_TYPE_>& that);

};


//
//  Constructor. Upon construction, this chain link is chained to
//  itself.
//
template <typename _TYPE_>
inline LinkType<_TYPE_>::LinkType(_TYPE_* object) :
    Link(object)
{
}


//
//  Destructor. Upon destruction, this chain link is removed from
//  any chain of which is may be a member.
//
template <typename _TYPE_>
inline LinkType<_TYPE_>::~LinkType() {
}


//
//  Return a pointer to the link on the chain after this link.
//
template <typename _TYPE_>
inline LinkType<_TYPE_>* LinkType<_TYPE_>::getNext() const {
    return static_cast<LinkType<_TYPE_>*>(this->Link::getNext());
}


//
//  Return a pointer to the object on the chain before this link.
//
template <typename _TYPE_>
inline LinkType<_TYPE_>* LinkType<_TYPE_>::getPrevious() const {
    return static_cast<LinkType<_TYPE_>*>(this->Link::getPrevious());
}


//
//  Return a pointer to the link that is the root of this link.
//
template <typename _TYPE_>
inline LinkType<_TYPE_>* LinkType<_TYPE_>::getRoot() const {
    return static_cast<LinkType<_TYPE_>*>(this->Link::getRoot());
}


//
//  Return a pointer to the payload on this link.
//
template <typename _TYPE_>
inline _TYPE_* LinkType<_TYPE_>::getPayload() const {
    return static_cast<_TYPE_*>(this->Link::getPayload());
}


//
//  Set the payload of this link to the specified pointer.
//
template <typename _TYPE_>
inline _TYPE_* LinkType<_TYPE_>::setPayload(_TYPE_* object) {
    return static_cast<_TYPE_*>(this->Link::setPayload(object));
}


//
//  Return true if this link has that link as its root.
//
template <typename _TYPE_>
inline bool LinkType<_TYPE_>::hasRoot(const LinkType<_TYPE_>* that) const {
    return this->Link::hasRoot(that);
}


//
//  Return true if this link is a root.
//
template <typename _TYPE_>
inline bool LinkType<_TYPE_>::isRoot() const {
    return this->Link::isRoot();
}


//
//  Remove this link from a chain.
//
template <typename _TYPE_>
inline LinkType<_TYPE_>* LinkType<_TYPE_>::remove() {
    return static_cast<LinkType<_TYPE_>*>(this->Link::remove());
}


//
//  Insert this link onto a chain just after that link. This
//  link inherits the root of that link.
//
template <typename _TYPE_>
inline LinkType<_TYPE_>* LinkType<_TYPE_>::insert(LinkType<_TYPE_>* that) {
    return static_cast<LinkType<_TYPE_>*>(this->Link::insert(that));
}


//
//  Reroot all links on the chain to this link.
//
template <typename _TYPE_>
inline LinkType<_TYPE_>* LinkType<_TYPE_>::reroot() {
    return static_cast<LinkType<_TYPE_>*>(this->Link::reroot());
}


//
//  Apply a functor to a chain of links, the functor deciding which
//  link to follow, next or previous.
//
template <typename _TYPE_>
LinkType<_TYPE_>*  LinkType<_TYPE_>::apply(Functor& functor) {
    LinkType<_TYPE_>* link = this;
    LinkType<_TYPE_>* result;

    while (true) {
        result = functor(link);
        if ((0 == result) || (link == result)) { return result; }
        link = result;
    }
}


//
//  Show this object on the output object.
//
template <typename _TYPE_>
void LinkType<_TYPE_>::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Link::show(level, display, indent + 1);
}


//
//  Virtual destructor for Functor.
//
template <typename _TYPE_>
LinkType<_TYPE_>::Functor::~Functor() {
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the LinkType unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestLinkType(void);
#endif


#endif
