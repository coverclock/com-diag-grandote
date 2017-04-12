#ifndef _COM_DIAG_DESPERADO_LOGOUTPUT_H_
#define _COM_DIAG_DESPERADO_LOGOUTPUT_H_

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
 *  Declares the LogOutput class.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/SyslogOutput.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a forwarding output functor that removes the log level encoding
 *  from its output buffer like SyslogOutput does.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class LogOutput : public SyslogOutput {

public:

    /**
     *  Constructor.
     *
     *	@param ro			refers to the output functor to which buffers are
     *						forwarded.
     */
    explicit LogOutput(
    	Output& ro
   );

    /**
     *  Destructor.
     */
    virtual ~LogOutput();

    using SyslogOutput::priority;

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
     *	Points to the output functor to which to forward log messages.
     */
    Output& ou;

};

#include "com/diag/desperado/End.h"


#endif
