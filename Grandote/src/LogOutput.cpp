/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the LogOutput class.
 *
 *  @see    LogOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/stdarg.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Logger.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
LogOutput::LogOutput(Output& ro)
: SyslogOutput("", 0, 0)
, ou(ro)
{
}


//
//  Destructor.
//
LogOutput::~LogOutput() {
    ou();
}


//
//  Output a character.
//
int LogOutput::operator() (int c) {
    return ou(c);
}


//
//  Format and output a variable length argument list.
//
ssize_t LogOutput::operator() (const char* format, va_list ap) {
    char buffer[minimum_buffer_size];
    ssize_t size = ::vsnprintf(buffer, sizeof(buffer), format, ap);
    int pri;
    const char* e = this->priority(buffer, size, pri);
    size -= e - buffer;
    return ou(e);
}


//
//  Output a string of no more than the specified size.
//
ssize_t LogOutput::operator() (const char* s, size_t size) {
	int pri;
    const char* e = this->priority(s, size, pri);
    size -= e - s;
    return ou(e, size);
}


//
//  Output binary data.
//
ssize_t LogOutput::operator() (
    const void* buffer,
    size_t minimum,
    size_t maximum 
) {
	int pri;
	const char* s = static_cast<const char*>(buffer);
    const char* e = this->priority(s, maximum, pri);
    maximum -= e - s;
    return ou(e, minimum, maximum);
}


//
//  Flush buffered data.
//
int LogOutput::operator() () {
    return ou();
}


//
//  Show this object on the output object.
//
void LogOutput::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->SyslogOutput::show(level, display, indent + 1);
    printf("%s ou:\n", sp);
    this->ou.show(level, display, indent + 2);
}


#include "com/diag/desperado/End.h"
