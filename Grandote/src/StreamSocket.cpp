/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Grandote library.
    
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
 *  Implements the StreamSocket class.
 *
 *  @see    StreamSocket
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/grandote/StreamSocket.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/Print.h"


namespace com { namespace diag { namespace grandote {


//
//  Constructor.
//
StreamSocket::StreamSocket(int socket) :
    in(socket),
    out(socket)
{
}


//
//  Constructor.
//
StreamSocket::StreamSocket(int reading, int writing) :
    in(reading),
    out(writing)
{
}


//
//  Destructor.
//
StreamSocket::~StreamSocket() {
}


//
//  Return a reference to the input interface.
//
Input& StreamSocket::input() {
    return this->in;
}


//
//  Return a reference to the output interface.
//
Output& StreamSocket::output() {
    return this->out;
}


//
//  Show this object on the output object.
//
void StreamSocket::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->InputOutput::show(level, display, indent + 1);
    printf("%s in:\n", sp);
    this->in.show(level, display, indent + 2);
    printf("%s out:\n", sp);
    this->out.show(level, display, indent + 2);
}


} } }
