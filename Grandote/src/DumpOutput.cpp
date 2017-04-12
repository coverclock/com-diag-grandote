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
 *  Implements the DumpOutput class.
 *
 *  @see    DumpOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/DumpOutput.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
DumpOutput::DumpOutput() :
    ou(0),
    du(0)
{
}


//
//  Constructor.
//
DumpOutput::DumpOutput(Output& ro) :
    ou(&ro),
    du(0)
{
}


//
//  Constructor.
//
DumpOutput::DumpOutput(Output& ro, Dump& rd) :
    ou(&ro),
    du(&rd)
{
}


//
//  Destructor.
//
DumpOutput::~DumpOutput() {
}


//
//  Returns output object.
//
Output& DumpOutput::output() const {
    return this->ou ? *this->ou : Platform::instance().output();
}


//
//  Returns dump object.
//
Dump& DumpOutput::dump() const {
    return this->du ? *this->du : Platform::instance().dump();
}


//
//  Return character.
//
int DumpOutput::operator() (int c) {
    Output& out = this->output();
    Dump& dmp = this->dump();
    dmp.bytes(&c, sizeof(c));
    return out(c);
}


//
//  Format a variable length argument list and return success.
//
ssize_t DumpOutput::operator() (const char* format, va_list ap) {
    Output& out = this->output();
    Dump& dmp = this->dump();
    char buffer[this->minimum_buffer_size + 1];
    ssize_t fc = ::vsnprintf(buffer, sizeof(buffer), format, ap);
    dmp.bytes(buffer, fc);
    return out(format, ap);
}


//
//  Return success.
//
ssize_t DumpOutput::operator() (const char* s, size_t size) {
    Output& out = this->output();
    Dump& dmp = this->dump();
    size_t sz = ::strnlen(s, size);
    dmp.bytes(s, sz);
    return out(s, size);
}


//
//  Return success.
//
ssize_t DumpOutput::operator() (
    const void* buffer,
    size_t minimum,
    size_t /* maximum */
) {
    Output& out = this->output();
    Dump& dmp = this->dump();
    dmp.words(buffer, minimum);
    return out(buffer, minimum, minimum);
}


//
//  Return success.
//
int DumpOutput::operator() () {
    Output& out = this->output();
    return out();
}


//
//  Show this object on the output object.
//
void DumpOutput::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Output::show(level, display, indent + 1);
    printf("%s ou=%p\n", sp, this->ou);
    if (this->ou) {
        this->ou->show(level, display, indent + 2);
    }
    printf("%s du=%p\n", sp, this->du);
    if (this->du) {
        this->du->show(level, display, indent + 2);
    }
}


#include "com/diag/desperado/End.h"
