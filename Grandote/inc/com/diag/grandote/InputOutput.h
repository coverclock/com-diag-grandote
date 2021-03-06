#ifndef _COM_DIAG_GRANDOTE_INPUTOUTPUT_H_
#define _COM_DIAG_GRANDOTE_INPUTOUTPUT_H_

/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2017 Digital Aggregates Corporation, Colorado, USA.
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
 *  Declares the InputOutput class.
 *
 *  @see    InputOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/grandote/Object.h"
#include "com/diag/grandote/Input.h"
#include "com/diag/grandote/Output.h"


namespace com { namespace diag { namespace grandote {

/**
 *  Defines a class that offers both an input functor and an
 *  output functor interface. The input and output functors of
 *  this base class are by default /dev/null-like Input and Output
 *  objects. The class can also be used as a container for an Input
 *  and an Output object (which do not have to be the same device).
 *  A derived class may implement more complex behavior such as
 *  reading to and writing from a common buffer, or (perhaps defining
 *  additional methods) accessing a database. The exact semantics of
 *  what it means to offer both input and output interfaces is
 *  strictly up to the derived class. For example, a ring buffer
 *  class derived from this class might allow data written into an
 *  object of this type by its output functor be retrieved later by
 *  reading from its input functor. But an interprocess communication
 *  class derived from this class might cause data written to be
 *  transmitted across a socket and consumed by another process, while
 *  the data read was that produced by the other process.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class InputOutput : public Object {

public:

    /**
     *  Construct an InputOutput container in which the
     *  Input and the Output are effectively /dev/null.
     */
    explicit InputOutput();

    /**
     * Construct an InputOutput container in which the
     * Input and Output functors will NOT be deleted by this
     * class' descructor.
     * @param in refers to an Input functor.
     * @param out refers to an Output functor.
     */
    explicit InputOutput(Input & in, Output & out) :
        inputr(in),
        outputr(out),
        inputp((Input *)0),
        outputp((Output *)0)
    {
    }

    /**
     * Construct an InputOutput container in which the
     * Input and Output functors WILL be deleted by this
     * class' descructor.
     * @param inp points to an Input functor.
     * @param outp points to an Output functor.
     */
    explicit InputOutput(Input * inp, Output * outp) :
        inputr(*inp),
        outputr(*outp),
        inputp(inp),
        outputp(outp)
    {
    }


    /**
     *  Destructor.
     */
    virtual ~InputOutput();

    /**
     *  Returns a reference to the input functor interface.
     *
     *  @return a reference to the input functor interface.
     */
    virtual Input& input();

    /**
     *  Returns a reference to the output functor interface.
     *
     *  @return a reference to the output functor interface.
     */
    virtual Output& output();

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

    Input & inputr;

    Output & outputr;

    Input * inputp;

    Output * outputp;


};

} } }


#endif
