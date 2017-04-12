#ifndef _COM_DIAG_DESPERADO_LOGGER_H_
#define _COM_DIAG_DESPERADO_LOGGER_H_

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
 *  Declares the Logger class.
 *
 *  @see    Logger
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/cxxcapi.h"


#if defined(__cplusplus)


#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Defines a simple interface to a multi-level logging mechanism, and
 *  implements a simple logger using it. Is easily overridden to use
 *  other logging mechanisms like cpp4j or syslog(3). The default behavior
 *  is to write output to the platform log functor. Different
 *  logging levels are supported. All levels of logging are enabled
 *  in the base class, although a derived class may implement a
 *  mechanism to limit logging to only certain levels. Levels
 *  representing the transitive closure of tools like log4j/log4cpp,
 *  Linux syslog, JDK Logger, and Apache SimpleLog are defined. A
 *  derived class may choose to map these to a smaller set of levels,
 *  or to extend the class to a larger set of levels. If you are into
 *  log4j or log4cpp, you can think of this class as a log4j logger, and its
 *  output functor as a log4j appender. The purpose of this class is
 *  not to provide an all purpose logging mechanism, but to allow
 *  code to be written that can later be adapted to an existing
 *  logging mechanism, perhaps in a legacy system.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Logger : public Object {

public:

    /**
     *  These are the levels available for log messages.
     *  They are the transitive closure of the levels
     *  available in syslog(3), log4j, log4cpp, JDK Logger,
     *  Apache SimpleLog, etc.
     */
    enum Level {
        FINEST          =   0,
        FINER           =   1,
        FINE            =   2,
        TRACE           =   3,
        DEBUG           =   4,
        INFORMATION     =   5,
        CONFIGURATION   =   6,
        NOTICE          =   7,
        WARNING         =   8,
        ERROR           =   9,
        SEVERE          =   10,
        CRITICAL        =   11,
        ALERT           =   12,
        FATAL           =   13,
        EMERGENCY       =   14,
        PRINT           =   15
    };

    /**
     * These characters delimit the symbolic extended log levels.
     */
    static const char
        LHS_EXTENDED = '[',
        RHS_EXTENDED = ']';

    /**
     * These characters delimit the numeric kernel log levels,
     * mimicing how the Linux kernel works.
     */
    static const char
        LHS_KERNEL = '<',
        RHS_KERNEL = '>';

    /**
     * This table maps from the kernel <0>..<7> log levels to the
     * extended log levels.
     */
    static const Level kernel[8];

    /**
     *  Given a log message in a buffer, determine its numeric level.
     *
     *  @param  buffer      points to the buffer containing
     *                      the message.
     *
     *  @param  size        is the maximum size of the buffer
     *                      in octets.
     *
     *  @param level		is the variable into which the result is returned.
     *
     *  @return a pointer in the buffer adjusted past the level encoding.
     */
    static const char* level(const char* buffer, size_t size, size_t & level);

    /**
     *  Constructor.
     *
     *  @param  po          points to an output object. If 0,
     *                      the platform log object is used.
     */
    explicit Logger(Output* po = 0);

    /**
     *  Constructor.
     *
     *  @param  ro          refers to an output object.
     */
    explicit Logger(Output& ro);

    /**
     *  Destructor.
     */
    virtual ~Logger();

    /**
     *  Returns a reference to its output object.
     *
     *  @return a reference to its output object.
     */
    virtual Output& output() const;

    /**
     *  This nul-terminated array contains labels corresponding to
     *  the levels.
     */
    static const char* labels[PRINT + 2];

    /**
     *  Return true if the specified level is enabled, false otherwise.
     *
     *  This method in the base class always returns true. It can be
     *  overridden in a derived class to support other behavior.
     *
     *  @param  level   indicates the level.
     *
     *  @return true if the specified level is enabled, false otherwise.
     */
    virtual bool isEnabled(Level level);

    /**
     *  Format a log message.
     *
     *  @param  buffer  points to the output buffer.
     *
     *  @param  size    is the size of the output buffer in bytes.
     *
     *  @param  level   indicates the level.
     *
     *  @param  format  points to the printf-style format string.
     *
     *  @param  ap      points to the argument list.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    virtual ssize_t format(
        char* buffer,
        size_t size,
        Level level,
        const char* format,
        va_list ap
    );

    /**
     *  Unconditionally emit a log message using a buffer.
     *
     *  @param  buffer  points to the buffer to be emitted.
     *
     *  @param  size    is the size of the buffer in bytes.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    virtual ssize_t emit(const char* buffer, size_t size);

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

    /**
     *  Log a message using an argument list if the specified level
     *  is enabled.
     *
     *  @param  level   indicates the level.
     *
     *  @param  format  points to the printf-style format string.
     *
     *  @param  ap      points to the argument list.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t vlog(Level level, const char* format, va_list ap);

    /**
     *  Log a message using a variadic argument list if the specified
     *  level is enabled.
     *
     *  @param  level   indicates the level.
     *
     *  @param  format  points to the printf-style format string.
     */
    ssize_t log(Level level, const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if FINEST is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t finest(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if FINER is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t finer(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if FINE is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t fine(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if TRACE is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t trace(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if DEBUG is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t debug(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if INFORMATION is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t information(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if CONFIGURATION is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t configuration(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if NOTICE is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t notice(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if WARNING is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t warning(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if ERROR is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t error(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if SEVERE is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t severe(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if CRITICAL is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t critical(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object is ALERT is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t alert(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if FATAL is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t fatal(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object if EMERGENCY is enabled.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t emergency(const char* format, ...);

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object unconditionally. The intent of the
     *  PRINT level is that it be used only to direct output to the
     *  log output functor that is the result of a craft demand
     *  (e.g. a command entered by a technician), not for log
     *  output generated spontaneously by the application.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    ssize_t print(const char* format, ...);

private:

    /**
     *  Points to the output object to which the logging is directed.
     */
    Output* ou;

};

#include "com/diag/desperado/End.h"


#else


/**
 *  This is an opaque type used to refer to a C++ output object
 *  in a C program.
 */
typedef struct Logger Logger;


#endif


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if FINEST is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_finest(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if FINER is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_finer(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if FINE is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_fine(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if TRACE is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_trace(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if DEBUG is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_debug(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if INFORMATION is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_information(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if CONFIGURATION is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_configuration(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if NOTICE is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_notice(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if WARNING is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_warning(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if ERROR is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_error(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if SEVERE is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_severe(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if CRITICAL is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_critical(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if ALERT is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_alert(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if FATAL is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_fatal(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if EMERGENCY is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_emergency(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


/**
 *  Formats a variadic argument list and writes the result
 *  to the specified log object if PRINT is enabled.
 *
 *  @param  logger      points to a logger object, for
 *                      example the platform logger object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t logger_print(CXXCTYPE(::com::diag::desperado::, Logger)* logger, const char* format, ...);


#if defined(DESPERADO_HAS_UNITTESTS)
/**
 *  Run the Logger unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestLogger(void);
/**
 *  Run the Logger CXXCAPI unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestLogger2(void);
#endif


#endif
