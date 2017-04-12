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
 *  Implements the SyslogOutput class.
 *
 *  @see    SyslogOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdarg.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/SyslogOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Logger.h"


#include "com/diag/desperado/Begin.h"


int SyslogOutput::priorities[] = {
#if defined(DESPERADO_HAS_SYSLOG)
	LOG_DEBUG,			// FINEST
	LOG_DEBUG,			// FINER
	LOG_DEBUG,			// FINE
	LOG_DEBUG,			// TRACE
    LOG_DEBUG,			// DEBUG
    LOG_INFO,			// INFORMATION
    LOG_NOTICE,			// CONFIGURATION
    LOG_NOTICE,			// NOTICE
    LOG_WARNING,		// WARNING
    LOG_ERR,			// ERROR
    LOG_CRIT,			// SEVERE
    LOG_CRIT,			// CRITICAL
    LOG_ALERT,			// ALERT
    LOG_EMERG,			// FATAL
    LOG_EMERG,			// EMERGENCY
    LOG_INFO			// PRINT
#else
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#endif
};

#if !defined(DESPERADO_HAS_SYSLOG)
/**
 *	This is the output functor used to print log messages to stderr.
 */
static FileOutput error(stderr);
#endif

//
//  Constructor.
//
SyslogOutput::SyslogOutput(const char* id, int opt, int fac)
: Output()
, ident(id)
, option(opt)
, facility(fac)
{
#if defined(DESPERADO_HAS_SYSLOG)
    ::openlog(id, opt, fac);
#endif
}


//
//  Destructor.
//
SyslogOutput::~SyslogOutput() {
#if defined(DESPERADO_HAS_SYSLOG)
    ::closelog();
#endif
}


//
// Map the embedded logging level to a priority.
//
const char* SyslogOutput::priority(const char* buffer, size_t size, int& pri) {
	size_t level;
	buffer = Logger::level(buffer, size, level);
    if (countof(this->priorities) <= level) {
    	level = countof(this->priorities) - 1;
    }
    pri = this->priorities[level];
    return buffer;
}


//
//  Output a character.
//
int SyslogOutput::operator() (int c) {
#if defined(DESPERADO_HAS_SYSLOG)
    ::syslog(LOG_INFO, "%c", c);
#else
    error(c);
#endif
   return c;
}


//
//  Format and output a variable length argument list.
//
ssize_t SyslogOutput::operator() (const char* format, va_list ap) {
    char buffer[minimum_buffer_size];
    ssize_t size = ::vsnprintf(buffer, minimum_buffer_size, format, ap);
    int pri;
    const char* e = this->priority(buffer, size, pri);
    size -= e - buffer;
#if defined(DESPERADO_HAS_SYSLOG)
    ::syslog(pri, "%s", e);
    return size;
#else
    return error(e);
#endif
}


//
//  Output a string of no more than the specified size.
//
ssize_t SyslogOutput::operator() (const char* s, size_t size) {
	int pri;
    const char* e = this->priority(s, size, pri);
    size -= e - s;
#if defined(DESPERADO_HAS_SYSLOG)
    ::syslog(pri, "%.*s", size, e);
    return size;
#else
    return error(e, size);
#endif
}


//
//  Output binary data.
//
ssize_t SyslogOutput::operator() (
    const void* buffer,
#if defined(DESPERADO_HAS_SYSLOG)
    size_t,
#else
    size_t minimum,
#endif
    size_t maximum 
) {
	int pri;
	const char* s = static_cast<const char*>(buffer);
    const char* e = this->priority(s, maximum, pri);
    maximum -= e - s;
#if defined(DESPERADO_HAS_SYSLOG)
   ::syslog(pri, "%.*s", maximum, e);
    return maximum;
#else
    return error(e, minimum, maximum);
#endif
}


//
//  Flush buffered data.
//
int SyslogOutput::operator() () {
#if defined(DESPERADO_HAS_SYSLOG)
    return 0;
#else
    return error();
#endif
}


//
//  Show this object on the output object.
//
void SyslogOutput::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Output::show(level, display, indent + 1);
    printf("%s ident=\"%s\"\n", sp, this->ident);
    printf("%s option=0x%x\n", sp, this->option);
    printf("%s facility=0x%x\n", sp, this->facility);
}


#include "com/diag/desperado/End.h"
