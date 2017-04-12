#ifndef _COM_DIAG_DESPERADO_PLATFORM_H_
#define _COM_DIAG_DESPERADO_PLATFORM_H_

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
 *  Declares the Platform class.
 *
 *  @see    Platform
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/types.h"
#include "com/diag/desperado/cxxcapi.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Logger.h"


#if defined(__cplusplus)


#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/DaylightSavingTime.h"
#include "com/diag/desperado/LeapSeconds.h"
#include "com/diag/desperado/Epoch.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Defines an interface to an underlying operating system.
 *  Although it may be used by applications using this library,
 *  its purpose is to provide a portable interface to
 *  the library itself to expedite porting the library to other
 *  operating system platforms.
 *
 *  If included from a C translation unit, defines a C-callable API.
 *
 *  @see    ISO, <I>Information technology - Portable Operating System
 *          Interface (POSIX) - Part 1: System Application Program Interface
 *          (API) [C Language]</I>, International Organization for
 *          Standardization, ISO/IEC 9945-1:1996(E), 1996
 *
 *  @see    H. Bruyninckx, <I>Real-Time and Embedded Guide</I>,
 *          Katholieke Universiteit Leuven, Leuven, Belgium, 2002-12-11,
 *          http://people.mech.kuleuven.be/~bruyninc/rthowto
 *
 *  @see    K. Obenland, <I>The Use of POSIX in Real-time Systems,
 *          Assessing its Effectiveness and Performance</I>,
 *          The MITRE Corporation
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Platform : public Object {

public:

    /**
     *  Allocates and constructs an instance of a derived class object and
     *  returns a base class reference to it. Exactly what kind of derived
     *  class object is implementation dependent. Passing this pointer as a
     *  reference to the instance method is sufficient to initialize the
     *  interface to the underlying platform. It is the responsibility of
     *  the application to provide any necessary mutual exclusion for this
     *  class method. Note that there is no C interface to this facility;
     *  a C++ main program must perform this action.
     *
     *  @return a reference to a platform object.
     */
    static Platform& factory();

    /**
     *  Sets the system platform object. Creating a system platform
     *  object and setting it using this class method should be done
     *  before using any of the other classes in this library. Passing
     *  a reference to a null pointer (if that's even possible) results
     *  in undefined behavior. It is the responsibility of the application
     *  to provide any necessary mutual exclusion for this class method.
     *  Note that there is no C interface to this facility; a C++ main
     *  program must perform this action.
     *
     *  @param  that        refers to the system platform object
     *                      which will be used for platform requests.
     */
    static void instance(Platform& that);

    /**
     *  Gets a reference to the system platform.
     *
     *  @return a reference to the system platform.
     */
    static Platform& instance();

    /**
     *  Destructor.
     */
    virtual ~Platform();

    /**
     *  Returns the frequency of the platform clock in Hertz as a
     *  ratio of two whole numbers. This ratio can represent frequencies
     *  that cannot represented as whole numbers.
     *
     *  @param  numerator       refers to a variable into which the numerator
     *                          of the ratio is returned.
     *
     *  @param  denominator     refers to a variable into which the denominator
     *                          of the ratio is returned.
     */
    virtual void frequency(ticks_t& numerator, ticks_t& denominator) = 0;

    /**
     *  Returns the frequency of the platform clock in Hertz as a
     *  whole number. Because platforms may have clock frequencies
     *  that are not easily represented as whole numbers, the integer
     *  returned by this method may be an approximation.
     *
     *  @return the frequency of the platform clock in Hertz.
     */
    virtual ticks_t frequency();

    /**
     *  Sets whether or not platform ticks incorporate leap seconds.
     *
     *  If leap second handling is enabled (true), then platform ticks
     *  are assumed to incorporate leap seconds, and hence are directly
     *  convertible into Common Era Seconds. If disabled (false), then
     *  platform ticks do not incorporate leap seconds, and are instead
     *  first conveted into atomic seconds.
     *
     *  @param  ls          is true if leap seconds are observed by
     *                      the platform ticks, false otherwise.
     */
    void setLeapSecondTicks(bool ls);

    /**
     *  Returns whether or not platform ticks incorporate leap seconds.
     *
     *  If leap second handling is enabled (true), then platform ticks
     *  are assumed to incorporate leap seconds, and hence are directly
     *  convertible into Common Era Seconds. If disabled (false), then
     *  platform ticks do not incorporate leap seconds, and are instead
     *  first conveted into atomic seconds.
     *
     *  @return true if platform ticks observe leap seconds, false
     *          otherwise.
     */
    bool getLeapSecondTicks() const;

    /**
     *  Sets the leap second rule.
     *
     *  The leap second rule controls how leap seconds are determined.
     *
     *  @param  rule        refers to a Leap Seconds rule. If Leap
     *                      seconds are not in use, the AtomicSeconds
     *                      rule can be used.
     */
    void setLeapSeconds(LeapSeconds& rule);

    /**
     *  Returns the leap second rule.
     *
     *  The leap second rule controls how leap seconds are determined.
     *
     *  @return a reference to the leap second rule.
     */
    LeapSeconds& getLeapSeconds() const;

    /**
     *  Sets the platform epoch as seconds since the Common Era epoch.
     *  The platform epoch may change as over time. Hence, successive time
     *  values may not be comparable. Time ticks may be converted to
     *  Common Era seconds, which are guaranteed to always be comparable.
     *
     *  Some platform may fix the platform epoch to a constant value,
     *  in which case this method does nothing.
     *
     *  @param  eh          refers to a platform epoch.
     */
    void setEpoch(const Epoch& eh);

    /**
     *  Returns the platform epoch as seconds since the Common Era epoch.
     *  The platform epoch may change as over time. Hence, successive time
     *  values may not be comparable. Time ticks may be converted to
     *  Common Era seconds, which are guaranteed to always be comparable.
     *
     *  @return a constant reference to the platform epoch.
     */
    const Epoch& getEpoch() const;

    /**
     *  Sets the time zone for the platform. The time zone is specified
     *  by signed seconds offset from universal coordinated time.
     *
     *  @param  ot          is the signed offset from UTC in seconds
     *                      (negative is East, positive is West).
     */
    void setOffset(int32_t ot);

    /**
     *  Returns the time zone for the platform expressed as a signed
     *  offset from coordinated universal time.
     *
     *  @return the signed offset from UTC in seconds.
     */
    int32_t getOffset() const;

    /**
     *  Sets the DST rule for the platform.
     *
     *  @param  rule        refers to a daylight saving time rule. If
     *                      daylight saving time is not in use, the
     *                      DstNever rule can be used.
     */
    void setDaylightSavingTime(DaylightSavingTime& rule);

    /**
     *  Returns the DST rule for the platform.
     *
     *  @return a reference to the current DST rule.
     */
    DaylightSavingTime& getDaylightSavingTime() const;

    /**
     *  Returns the time of day in absolute ticks since the platform epoch.
     *  The platform epoch is the time and date from which all time measurement
     *  is relative. Different platforms use different epochs. The platform
     *  epoch is not necessarily the same thing as the Common Era epoch
     *  (although on some platforms it may be).
     *
     *  @return the time of day in absolute ticks of duration since
     *          the platform epoch.
     */
    virtual ticks_t time()= 0;

    /**
     *  Returns the elapsed time in absolute ticks since this object
     *  was constructed.
     *
     *  @return the relative time in relative ticks of duration since
     *          this object was constructed.
     */
    virtual ticks_t elapsed() = 0;

    /**
     *  Yields the processor at least the specified number of ticks by
     *  delaying the calling thread. If zero ticks are specified, the
     *  context is  guaranteed to still relinquish the processor.
     *
     *  @param  ticks       is the number of ticks to yield the processor.
     *
     *  @param  premature   is true if the yield can be interrupted by
     *                      an asynchronous event, false if the event
     *                      should be ignored until the specified number
     *                      of ticks have elapsed.
     *
     *  @return the actual number of ticks delayed, which nominally
     *          should be equal to or greater then the requested amount
     *          depending on the scheduling granularity and whether an
     *          asynchronous event caused the method to return prematurely.
     */
    virtual ticks_t yield(ticks_t ticks = 0, bool premature = true) = 0;

    /**
     *  Returns the identity of the caller. Since Desperado requires
     *  a POSIX interface including POSIX Threads, this is typically
     *  the process ID from getpid(2) appended with the thread ID
     *  from pthread_self(3). However, other platforms may implement
     *  other identifiers.
     *
     *  @return the identity of the caller.
     */
    virtual identity_t identity() = 0;

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
    ) = 0;

    /**
     *  Returns a reference to the platform input functor.
     *
     *  @return a reference to the platform input functor.
     */
    virtual Input& input() = 0;

    /**
     *  Returns a reference to the platform output functor.
     *
     *  @return a reference to the platform output functor.
     */
    virtual Output& output() = 0;

    /**
     *  Returns a reference to the platform error output functor.
     *
     *  @return a reference to the platform error output functor.
     */
    virtual Output& error() = 0;

    /**
     *  Returns a reference to the platform log output functor.
     *
     *  @return a reference to the platform log output functor.
     */
    virtual Output& log() = 0;

    /**
     *  Returns a reference to the platform dump object. Output
     *  from this dump object will go to the platform error output
     *  object.
     *
     *  @return a reference to the platform dump object.
     */
    virtual Dump& dump() = 0;

    /**
     *  Returns a reference to the platform heap object.
     *
     *  @return a reference to the platform heap object.
     */
    virtual Heap& heap() = 0;

    /**
     *  Returns a reference to the platform print object. Output
     *  from this print object will go to the platform error output
     *  object.
     *
     *  @return a reference to the platform print object.
     */
    virtual Print& print() = 0;

    /**
     *  Returns a reference to the platform logger object. Output
     *  from this logger object will go to the platform log output
     *  object.
     *
     *  @return a reference to the platform logger object.
     */
    virtual Logger& logger() = 0;

    /**
     *  Returns the name of this platform.
     *
     *  @return the name of this platform.
     */
    virtual const char* platform() = 0;

    /**
     *  Returns the name of the target on which this platform is running.
     *
     *  @return the name of the target on which this platform is running.
     */
    virtual const char* target() = 0;

    /**
     *  Returns the name of the host on which this platform is running.
     *
     *  @return the name of the host on which this platform is running.
     */
    virtual const char* host() = 0;

    /**
     *  Extracts the component name from a translation unit path and copies
     *  it into the specified buffer. A pointer to the buffer is returned.
     *  For example, the component name of "/home/coverclock/src/Platform.C"
     *  is "Platform". This is a platform method because the extraction
     *  of the platform name from the name of the translation unit is
     *  platform dependent.
     *
     *  @param  path    is the path name, typically __FILE__.
     *
     *  @param  buffer  is the buffer.
     *
     *  @param  size    is the size of the buffer in octets.
     *
     *  @return a pointer to the buffer.
     */
    virtual char* component(const char* path, char* buffer, size_t size) const = 0;

    /**
     *  Returns a pointer to the errno variable appropriate for the calling
     *  context.
     *
     *  @return a pointer to the errno variable.
     */
    virtual int* errornumber() = 0;

    /**
     *  Returns the maximum errno value for this platform. By using this
     *  platform method, an application can provide its own messages when
     *  the errno value is out of range. This may not be the most efficient
     *  code one some platforms; it is a good idea to just call it once during
     *  initialization and cache the value.
     *
     *  @return the maximum errno value.
     */
    virtual int errormaximum() = 0;

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
    ) = 0;

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

protected:

    /**
     *  Pointer to the system platform object.
     */
    static Platform* singleton;

    /**
     *  Constructor.
     */
    explicit Platform();

private:

    /**
     *  Copy constructor.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Platform(const Platform& that);

    /**
     *  Assignment operator.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Platform& operator=(const Platform& that);

    /**
     *  This is true if platform ticks incorporate leap seconds,
     *  false otherwise.
     */
    bool leapseconds;

    /**
     *  This points to the Leap Seconds rule in effect. There
     *  should be a leap second rule regardless of whether platform
     *  ticks incorporate leap seconds.
     */
    LeapSeconds* lsrule;

    /**
     *  This is the platform epoch. Note that this is a copy of
     *  the object and not a pointer or a reference to it.
     */
    Epoch epoch;

    /**
     *  This is the signed offset from coordinated universal time in
     *  seconds.
     */
    int32_t offset;

    /**
     *  This points to the daylight saving time rule in effect.
     */
    DaylightSavingTime* dstrule;

};


//
//  Set leap seconds ticks true or false.
//
inline void Platform::setLeapSecondTicks(bool ls) {
    this->leapseconds = ls;
}


//
//  Return leap seconds ticks true or false.
//
inline bool Platform::getLeapSecondTicks() const {
    return this->leapseconds;
}


//
//  Set leap second rule.
//
inline void Platform::setLeapSeconds(LeapSeconds& rule) {
    this->lsrule = &rule;
}


//
//  Return leap second rule.
//
inline LeapSeconds& Platform::getLeapSeconds() const {
    return *this->lsrule;
}


//
//  Set epoch.
//
inline void Platform::setEpoch(const Epoch& eh) {
    this->epoch = eh;
}


//
//  Return epoch.
//
inline const Epoch& Platform::getEpoch() const {
    return this->epoch;
}


//
//  Set time zone offset.
//
inline void Platform::setOffset(int32_t ot) {
    TimeZone zone;
    this->offset = zone.normalize(ot);
}


//
//  Get time zone offset.
//
inline int32_t Platform::getOffset() const {
    return this->offset;
}


//
//  Set the DST rule.
//
inline void Platform::setDaylightSavingTime(DaylightSavingTime& rule) {
    this->dstrule = &rule;
}


//
//  Get DST rule.
//
inline DaylightSavingTime& Platform::getDaylightSavingTime() const {
    return *this->dstrule;
}

#include "com/diag/desperado/End.h"

#endif


/**
 *  Returns the frequency of the platform clock in Hertz as a
 *  whole number. Because platforms may have clock frequencies
 *  that are not easily represented as whole numbers, the integer
 *  returned by this method may be an approximation.
 *
 *  @return the frequency of the platform clock in Hertz.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, ticks_t) platform_frequency(void);


/**
 *  Returns the time of day in absolute ticks since the platform epoch.
 *
 *  @return the time of day in absolute ticks of duration since
 *          the platform epoch.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, ticks_t) platform_time(void);


/**
 *  Returns the elapsed time in absolute ticks since this object
 *  was constructed.
 *
 *  @return the relative time in relative ticks of duration since
 *          this object was constructed.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, ticks_t) platform_elapsed(void);


/**
 *  Yields the processor at least the specified number of ticks by
 *  delaying the calling thread.  If zero ticks are specified, the
 *  context is  guaranteed to still relinquish the processor.
 *
 *  @param  ticks       is the number of ticks to yield the processor.
 *
 *  @param  premature   is true if the yield can be interrupted by
 *                      an asynchronous event, false if the event
 *                      should be ignored until the specified number
 *                      of ticks have elapsed.
 *
 *  @return the actual number of ticks delayed, which nominally
 *          should be equal to or greater then the requested amount
 *          depending on the scheduling granularity and whether an
 *          asynchronous event caused the method to return prematurely.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, ticks_t) platform_yield(CXXCTYPE(::com::diag::desperado::, ticks_t) ticks, int premature);


/**
 *  Returns the identity of the caller. Since Desperado requires
 *  a POSIX interface including POSIX Threads, this is typically
 *  the process ID from getpid(2) appended with the thread ID
 *  from pthread_self(3). However, other platforms may implement
 *  other identifiers.
 *
 *  @return the identity of the caller.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, identity_t) platform_identity();


/**
 *  Invokes the platform fatal method.
 *
 *  @see Platform::fatal()
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
CXXCAPI void platform_fatal(
    const char* event,
    int error,
    const char* file,
    int line,
    const char* function
);


/**
 *  Returns a pointer to the platform input object.
 *
 *  @see Platform::input()
 *
 *  @return a pointer to the platform input object.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, Input)* platform_input(void);


/**
 *  Returns a pointer to the platform output object.
 *
 *  @see Platform::output()
 *
 *  @return a pointer to the platform output object.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, Output)* platform_output(void);


/**
 *  Returns a pointer to the platform error output object.
 *
 *  @see Platform::error()
 *
 *  @return a pointer to the platform error output object.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, Output)* platform_error(void);


/**
 *  Returns a pointer to the platform dump object.
 *
 *  @see Platform::dump()
 *
 *  @return a pointer to the platform dump object.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, Dump)* platform_dump(void);


/**
 *  Returns a pointer to the platform heap object.
 *
 *  @see Platform::heap()
 *
 *  @return a pointer to the platform heap object.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, Heap)* platform_heap(void);


/**
 *  Returns a pointer to the platform logger object.
 *
 *  @see Platform::logger()
 *
 *  @return a pointer to the platform logger object.
 */
CXXCAPI CXXCTYPE(::com::diag::desperado::, Logger)* platform_logger(void);


/**
 *  Returns the name of this platform.
 *
 *  @return the name of this platform.
 */
CXXCAPI const char* platform_platform();


/**
 *  Returns the name of the target on which this platform is running.
 *
 *  @return the name of the target on which this platform is running.
 */
CXXCAPI const char* platform_target();


/**
 *  Returns the name of the host on which this platform is running.
 *
 *  @return the name of the host on which this platform is running.
 */
CXXCAPI const char* platform_host();


/**
 *  Performs a formatted print to the platform print object.
 *
 *  @see Platform::print()
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t platform_printf(const char* format, ...);


/**
 *  Performs a formatted print to the platform error object.
 *
 *  @see Platform::error()
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI ssize_t platform_errorf(const char* format, ...);


/**
 *  Returns a pointer to the errno variable appropriate
 *  for the calling context.
 *
 *  @return a pointer to the errno variable.
 */
CXXCAPI int* platform_errornumber(void);


/**
 *  Returns the maximum errno value.
 *
 *  @return the maximum errno value.
 */
CXXCAPI int platform_errormaximum(void);


/**
 *  Returns a pointer to an error message associated with the specified
 *  error number.
 *
 *  @param  errndx      is the errno value.
 *
 *  @param  buffer      is a buffer into which the error message may
 *                      written if it must be generated.
 *
 *  @param  buflen      is the sizeof the buffer.
 *
 *  @return a pointer to an error message.
 */
CXXCAPI const char* platform_errormessage(
    int errndx,
    char* buffer,
    size_t buflen
);


#if defined(DESPERADO_HAS_UNITTESTS)
/**
 *  Run the Platform unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestPlatform(void);
/**
 *  Run the Platform CXXCAPI unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestPlatform2(void);
#endif


#endif
