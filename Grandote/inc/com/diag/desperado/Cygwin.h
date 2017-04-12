#ifndef _COM_DIAG_DESPERADO_CYGWIN_H_
#define _COM_DIAG_DESPERADO_CYGWIN_H_

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
 *  Declares the Cygwin class.
 *
 *  @see    Cygwin
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/Logger.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements the Platform interface to the Windows operating system
 *  using Cygwin.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Cygwin : public Platform {

public:

    /**
     *  Constructor.
     */
    explicit Cygwin();

    /**
     *  Destructor.
     */
    virtual ~Cygwin();

    using Platform::frequency;

    /**
     *  Returns the frequency of the Cygwin clock in Hertz as a
     *  a ratio of a numerator and a denominator.
     *
     *  @param  numerator       refers to a variable into which
     *                          the numerator is returned.
     *
     *  @param  denominator     refers to a variable into which
     *                          the denominator is returned.
     */
    virtual void frequency(ticks_t& numerator, ticks_t& denominator);

    /**
     *  Returns the time of day in Cygwin ticks since the epoch.
     *
     *  @return the time of day in Cygwin ticks since the epoch.
     */
    virtual ticks_t time();

    /**
     *  Returns the elapsed time in relative ticks since this object
     *  was constructed.
     *
     *  @return the relative time in relative ticks of duration since
     *          this object was constructed.
     */
    virtual ticks_t elapsed();

    /**
     *  Yields the processor at least the specified number of Cygwin ticks
     *  by delaying the calling thread.
     *
     *  @param  ticks       is the number of Cygwin ticks to yield.
     *
     *  @param  premature   is true if the yield can be interrupted by
     *                      an signal, false if the signal should be
     *                      ignored until the specified number of
     *                      ticks have elapsed.
     *
     *  @return the actual number of ticks delayed, which nominally
     *          should be equal to or greater then the requested amount
     *          depending on whether a signal caused the method to return
     *          prematurely.
     */
    virtual ticks_t yield(ticks_t ticks = 0, bool premature = true);

    /**
     *  Returns the identity of the caller. Since Desperado requires
     *  a POSIX interface including POSIX Threads, this is typically
     *  the process ID from getpid(2) appended with the thread ID
     *  from pthread_self(3). However, other platforms may implement
     *  other identifiers.
     */
    virtual identity_t identity();

    /**
     *  Signifies that fatal error has occurred. What happens next,
     *  and in what context it occurs, is platform dependent.
     *
     *  @param  event           is a message associated with the
     *                          fatal error or null (0).
     *
     *  @param  error           is an error number that may be associated
     *                          with the fatal error. If zero, no errno
     *                          information is processed. If negative, the
     *                          system errno value is used.
     *
     *  @param  file            may be the file name of the issuing
     *                          translation unit or null (0).
     *
     *  @param  line            may be the line number in the issuing
     *                          translation unit.
     *
     *  @param  function        may be the function name in the
     *                          issuing translation unit or null (0).
     */
    virtual void fatal(
        const char* event = 0,
        int error = -1,
        const char* file = 0,
        int line = 0,
        const char* function = 0
    );

    /**
     *  Returns a reference to the Cygwin input functor.
     *
     *  @return a reference to the Cygwin input functor.
     */
    virtual Input& input();

    /**
     *  Returns a reference to the Cygwin output functor.
     *
     *  @return a reference to the Cygwin output functor.
     */
    virtual Output& output();

    /**
     *  Returns a reference to the Cygwin error output functor.
     *
     *  @return a reference to the Cygwin error output functor.
     */
    virtual Output& error();

    /**
     *  Returns a reference to the Cygwin log output functor.
     *  Data written to the log output functor go to the
     *  system log (syslog) with a priority of LOG_ERR.
     *
     *  @return a reference to the Cygwin log output functor.
     */
    virtual Output& log();

    /**
     *  Returns a reference to the Cygwin dump object. Output
     *  from this dump object will go to the Cygwin error output
     *  object.
     *
     *  @return a reference to the Cygwin dump object.
     */
    virtual Dump& dump();

    /**
     *  Returns a reference to the Cygwin heap object.
     *
     *  @return a reference to the Cygwin heap object.
     */
    virtual Heap& heap();

    /**
     *  Returns a reference to the Cygwin print object. Output
     *  from this print object will go to the Cygwin error output
     *  object.
     *
     *  @return a reference to the Cygwin print object.
     */
    virtual Print& print();

    /**
     *  Returns a reference to the Cygwin logger object. Output
     *  from this print object will go to the Cygwin log output
     *  object.
     *
     *  @return a reference to the Cygwin logger object.
     */
    virtual Logger& logger();

    /**
     *  Returns the name of this platform.
     *
     *  @return the name of this platform.
     */
    virtual const char* platform();

    /**
     *  Returns the name of the target on which this platform is running.
     *
     *  @return the name of the target on which this platform is running.
     */
    virtual const char* target();

    /**
     *  Returns the name of the host on which this platform is running.
     *
     *  @return the name of the host on which this platform is running.
     */
    virtual const char* host();

    /**
     *  Extracts the component name from a translation unit path and copies
     *  it into the specified buffer. A pointer to the buffer is returned.
     *  For example, the component name of "/home/coverclock/src/Platform.C"
     *  is "Platform".
     *
     *  @param  path    is the path name, typically __FILE__.
     *
     *  @param  buffer  is the buffer.
     *
     *  @param  size    is the size of the buffer in octets.
     *
     *  @return a pointer to the buffer.
     */
    virtual char* component(const char* path, char* buffer, size_t size) const;

    /**
     *  Returns a pointer to the errno variable appropriate for the calling
     *  context.
     *
     *  @return a pointer to the errno variable.
     */
    virtual int* errornumber();

    /**
     *  Returns the maximum errno value for this platform. By using this
     *  platform method, an application can provide its own messages when
     *  the errno value is out of range. This may not be the most efficient
     *  code one some platforms; it is a good idea to just call it once during
     *  initialization and cache the value.
     *
     *  @return the maximum errno value.
     */
    virtual int errormaximum();

    /**
     *  Returns a pointer to the error message (with no trailing newline)
     *  associated with the specified errno. Typically this indexes an
     *  array of character strings provided by the underlying platform.
     *  By overriding this platform method, an application can extend
     *  the platform-provided error message array by providing its
     *  own messages when the errno value is out of range.
     *
     *  @param  errndx  is the errno value used to index the array. If
     *                  it is beyond the countof the array, a default
     *                  error message is returned.
     *
     *  @param  buffer  is a buffer into which the error message may
     *                  written if it must be generated.
     *
     *  @param  buflen  is the sizeof the buffer.
     *
     *  @return a pointer to a character string containing an error message.
     */
    virtual const char* errormessage(
        int errndx,
        char* buffer,
        size_t buflen
    );

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
     *                  sent. If null (zero), the default Cygwin output
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
    Cygwin(const Cygwin& that);

    /**
     *  Assignment operator.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Cygwin& operator=(const Cygwin& that);

    /**
     *  This is the time this object was constructed.
     */
    ticks_t birthdate;

    /**
     *  This is the platform standard input object.
     */
    FileInput inputs;

    /**
     *  This is the platform standard output object.
     */
    FileOutput outputs;

    /**
     *  This is the platform standard error object.
     */
    FileOutput errors;

    /**
     *  This is the platform dump object.
     */
    Dump dumps;

    /**
     *  This is the platform heap object.
     */
    Heap heaps;

    /**
     *  This is the platform print object.
     */
    Print prints;

    /**
     *  This is the platform logger object.
     */
    Logger loggers;

};

#include "com/diag/desperado/End.h"


#endif
