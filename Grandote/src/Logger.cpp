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
 *  Implements the Logger class.
 *
 *  @see    Logger
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdarg.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/TimeStamp.h"


#include "com/diag/desperado/Begin.h"


const char* Logger::labels[] = {
    "FINS",
    "FINR",
    "FINE",
    "TRAC",
    "DBUG",
    "INFO",
    "CONF",
    "NOTE",
    "WARN",
    "ERRR",
    "SEVR",
    "CRIT",
    "ALRT",
    "FATL",
    "EMER",
    "PRNT",
    0
};

const Logger::Level Logger::kernel[] = {
    Logger::EMERGENCY,      // KERN_EMERG
    Logger::ALERT,          // KERN_ALERT
    Logger::CRITICAL,       // KERN_CRIT
    Logger::ERROR,          // KERN_ERR
    Logger::WARNING,        // KERN_WARNING
    Logger::NOTICE,         // KERN_NOTICE
    Logger::INFORMATION,    // KERN_INFORMATION
    Logger::DEBUG           // KERN_DEBUG
};


//
//  Return the logging level encoded (or not) in the buffer.
//  This is a hack, but like many hacks, it works.
//
const char* Logger::level(const char* buffer, size_t size, size_t & level) {

	level = PRINT;

    do {

        if (sizeof("[X]") > size) {
            break;
        }

        // We encode our four-bit logging level at the beginning of the
        // buffer in the form of "[X]" where X is a hex digit.

        if (buffer[0] != LHS_EXTENDED) {
            // Do nothing.
        } else if (buffer[2] != RHS_EXTENDED) {
            // Do nothing.
        } else if (('0' <= buffer[1]) && (buffer[1] <= '9')) {
        	level = buffer[1] - '0' + 0;
            buffer += sizeof("[X]") - 1;
            break;
        } else if (('a' <= buffer[1]) && (buffer[1] <= 'f')) {
        	level = buffer[1] - 'a' + 0xa;
            buffer += sizeof("[X]") - 1;
            break;
        } else if (('A' <= buffer[1]) && (buffer[1] <= 'F')) {
        	level = buffer[1] - 'A' + 0xA;
            buffer += sizeof("[X]") - 1;
            break;
        } else {
        	break;
        }

        // The kernel and other related logging mechanisms encode a three-bit
        // logging level at the beginning of the buffer in the form of "<D>"
        // where D is a decimal digit.

        if (buffer[0] != LHS_KERNEL) {
            // Do nothing.
        } else if (buffer[2] != RHS_KERNEL) {
            // Do nothing.
        } else if (('0' <= buffer[1]) && (buffer[1] <= '7')) {
        	level = kernel[buffer[1] - '0'];
            buffer += sizeof("<D>") - 1;
            break;
        } else {
        	break;
        }

    } while (false);

    return buffer;
}

//
//  Constructor.
//
Logger::Logger(Output* po) :
    ou(po)
{
}


//
//  Constructor.
//
Logger::Logger(Output& ro) :
    ou(&ro)
{
}


//
//  Destructor.
//
Logger::~Logger() {
}


//
//  Returns current output object.
//
Output& Logger::output() const {
    return this->ou ? *this->ou : Platform::instance().log();
}


//
//  All levels are always enabled in the base class.
//
bool Logger::isEnabled(Level /* level */) {
    return true;
}


//
//  Format the log string with a prefix into the provided buffer.
//
ssize_t Logger::format(
    char* buffer,
    size_t size,
    Level level,
    const char* format,
    va_list ap
) {
    TimeStamp timestamp;
    const char* stamp = timestamp.log();
    if (!((0 <= level) &&
          (static_cast<size_t>(level) < countof(this->labels)))) {
        level = Logger::PRINT;
    }
    int octets = ::snprintf(buffer, size, "[%x]%s [%4.4s] ",
            level, stamp, this->labels[level]);
    int rc = ::vsnprintf(buffer + octets, size - octets, format, ap);
    return octets + rc;
}


//
//  Emit the log string.
//
ssize_t Logger::emit(const char* buffer, size_t size) {
    Output& out = this->output();
    ssize_t rc = out(buffer, size);
    out();
    return rc;
}


//
//  Note that the PRINT level is checked for explicitly, enforcing the
//	fact that the PRINT level is logged unconditionally regardless of
//	the value returned by the isEnabled() method.
//
#define DESPERADO_LOGGER_BODY(_LEVEL_) \
	do { \
		if ((this->PRINT == _LEVEL_) || this->isEnabled(_LEVEL_)) { \
			char buffer[Output::minimum_buffer_size]; \
			ssize_t size = this->format(buffer, sizeof(buffer), _LEVEL_, format, ap); \
			rc = this->emit(buffer, size); \
		} \
	} while (false)


ssize_t Logger::vlog(Level level, const char* format, va_list ap) {
    ssize_t rc = 0;
    DESPERADO_LOGGER_BODY(level);
    return rc;
}


//
//  Log the variadic argument list.
//
ssize_t Logger::log(Level level, const char* format, ...) {
	ssize_t rc = 0;
    va_list ap;
    va_start(ap, format);
    DESPERADO_LOGGER_BODY(level);
    va_end(ap);
    return rc;
}


#define DESPERADO_LOGGER(_FUNCTION_, _LEVEL_) \
ssize_t Logger::_FUNCTION_(const char* format, ...) { \
    ssize_t rc = 0; \
    va_list ap; \
    va_start(ap, format); \
    DESPERADO_LOGGER_BODY(_LEVEL_); \
    va_end(ap); \
    return rc; \
}


DESPERADO_LOGGER(   finest,         FINEST          )
DESPERADO_LOGGER(   finer,          FINER           )
DESPERADO_LOGGER(   fine,           FINE            )
DESPERADO_LOGGER(   trace,          TRACE           )
DESPERADO_LOGGER(   debug,          DEBUG           )
DESPERADO_LOGGER(   information,    INFORMATION     )
DESPERADO_LOGGER(   configuration,  CONFIGURATION   )
DESPERADO_LOGGER(   notice,         NOTICE          )
DESPERADO_LOGGER(   warning,        WARNING         )
DESPERADO_LOGGER(   error,          ERROR           )
DESPERADO_LOGGER(   severe,         SEVERE          )
DESPERADO_LOGGER(   critical,       CRITICAL        )
DESPERADO_LOGGER(   alert,          ALERT           )
DESPERADO_LOGGER(   fatal,          FATAL           )
DESPERADO_LOGGER(   emergency,      EMERGENCY       )
DESPERADO_LOGGER(   print,          PRINT           )


//
//  Show this object on the output object.
//
void Logger::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s ou=%p\n", sp, this->ou);
    if (this->ou) {
        this->ou->show(level, display, indent + 2);
    }
}


#include "com/diag/desperado/End.h"
