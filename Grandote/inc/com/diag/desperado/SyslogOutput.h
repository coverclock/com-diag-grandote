#ifndef _COM_DIAG_DESPERADO_SYSLOGOUTPUT_H_
#define _COM_DIAG_DESPERADO_SYSLOGOUTPUT_H_

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
 *  Declares the SyslogOutput class.
 *
 *  @see    SyslogOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/Platform.h"
#if defined(DESPERADO_HAS_SYSLOG)
#include <syslog.h>
#define SYSLOG_OUTPUT_OPT	(LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID)
#define SYSLOG_OUTPUT_FAC	(LOG_USER)
#else
#define SYSLOG_OUTPUT_OPT	(0)
#define SYSLOG_OUTPUT_FAC	(0)
#endif
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an output functor that writes to the Linux system log file
 *  using syslog(3), conspiring with the Logger class to pass along
 *  the priority of each log message.
 *
 *  @see    syslog(3)
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class SyslogOutput : public Output {

public:

    /**
     *  This table contains syslog priorities indexed by the corresponding
     *  logger level.
     */
    static int priorities[Logger::PRINT + 1];

    /**
     *  Constructor.
     *
     *  @param  id          is the syslog ident.
     *
     *  @param  opt         is the syslog option.
     *
     *  @param  fac         is the syslog facility.
     */
    explicit SyslogOutput(
        const char* id = "SyslogOutput",
        int opt = SYSLOG_OUTPUT_OPT,
        int fac = SYSLOG_OUTPUT_FAC
   );

    /**
     *  Destructor. The syslog is closed for this application when the
     *  object is destroyed.
     */
    virtual ~SyslogOutput();

    /**
     *  Returns the syslog ident.
     *
     *  @return the syslog ident.
     */
    const char* getIdent() const;

    /**
     *  Returns the syslog option.
     *
     *  @return the syslog option.
     */
    int getOption() const;

    /**
     *  Returns the syslog facility.
     *
     *  @return the syslog facility.
     */
    int getFacility() const;

    /**
     *  Determines the syslog priority of the given message by
     *  conspiring with this object's logger.
     *
     *  @param  buffer      points to the buffer containing the message.
     *
     *  @param  size        is the size of the message in octets.
     *
     *  @param pri			is the variable into which the priority is returned.
     *
     *  @return a pointer to an adjusted position in the buffer.
     */
    const char* priority(const char* buffer, size_t size, int& pri);

    /**
     *  Outputs a character in integer form to the syslog.
     *
     *  @param  c           is a character in integer form.
     *
     *  @return the output character if successful, EOF otherwise.
     */
    virtual int operator() (int c);

    /**
     *  Formats a variable length argument list and output the result
     *  to the syslog.
     *
     *  @param  format      is a NUL-terminated string containing a
     *                      printf-style format statement.
     *
     *  @param  ap          is a variable length argument object.
     *
     *  @return a non-negative number if successful, EOF otherwise.
     */
    virtual ssize_t operator() (const char* format, va_list ap);

    /**
     *  Outputs a string of no more than the specified length not
     *  including its terminating NUL to the syslog.
     *
     *  @param  s           points to constant NUL-terminated string.
     *
     *  @param  size        is the size of the string in octets.
     *
     *  @return a non-negative number if successful, EOF otherwise.
     */
    virtual ssize_t operator() (
        const char* s,
        size_t size = maximum_string_length
    );

    /**
     *  Outputs binary data from a buffer to the syslog.
     *
     *  @param  buffer  points to the buffer.
     *
     *  @param  minimum is the minimum number of octets to output.
     *
     *  @param  maximum is the maximum number of octets to output.
     *
     *  @return a non-negative number if successful, EOF otherwise.
     */
    virtual ssize_t operator() (
        const void* buffer,
        size_t minimum,
        size_t maximum
    );

    /**
     *  Flush any buffered data to the syslog.
     *
     *  @return a non-negative number if successful, EOF otherwise.
     */
    virtual int operator() ();

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
     *  This is the ident used to open the syslog.
     */
    const char* ident;

    /**
     *  This is the option used to open the syslog.
     */
    int option;

    /**
     *  This is the facility used to open the syslog.
     */
    int facility;

};


//
//  Return the ident.
//
inline const char* SyslogOutput::getIdent() const {
    return this->ident;
}


//
//  Return the option.
//
inline int SyslogOutput::getOption() const {
    return this->option;
}


//
//  Return the facility.
//
inline int SyslogOutput::getFacility() const {
    return this->facility;
}


#include "com/diag/desperado/End.h"


#endif
