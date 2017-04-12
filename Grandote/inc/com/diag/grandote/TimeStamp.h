#ifndef _COM_DIAG_DESPERADO_TIMESTAMP_H_
#define _COM_DIAG_DESPERADO_TIMESTAMP_H_

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
 *  Declares the TimeStamp class.
 *
 *  @see    TimeStamp
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/LocalTime.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements methods to represent CommonEra and LocalTime dates and times
 *  as printable timestamps in a variety of formats. The buffer into which
 *  the timestamp is formatted is internal to this object, so if you, for
 *  example, want to print several different timestamps in a single output
 *  statement, you must use several objects of this type. The buffer is
 *  guaranteed to be nul terminated and to be large enough to contain
 *  any of the supported timestamps.
 *
 *  @see    CommonEra
 *
 *  @see    LocalTime
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class TimeStamp : public Object {

public:

    /**
     *  Constructor.
     */
    explicit TimeStamp();

    /**
     *  Destructor.
     */
    virtual ~TimeStamp();

    /**
     *  Print a timestamp conforming to ISO8601, RFC3339 and W3C NOTE-datetime
     *  using the current platform time.
     *
     *  Example: "2005-07-05T15:48:10Z".
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* iso8601();

    /**
     *  Print a timestamp conforming to ISO8601, RFC3339 and W3C NOTE-datetime
     *  using a CommonEra date and time.
     *
     *  Example: "2005-07-05T15:48:10Z".
     *
     *  @param  ce      refers to a Common Era object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* iso8601(const CommonEra& ce);

    /**
     *  Print a timestamp conforming to ISO8601 RFC3339 and W3C NOTE-datetime
     *  using a LocalTime date and time.
     *
     *  Example: "2005-07-05T15:48:10-07:00".
     *
     *  @param  lt      refers to a Local Time object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* iso8601(const LocalTime& lt);

    /**
     *  Print an milspec-style timestamp using the current platform
     *  time.
     *
     *  Example: "2005-Jul-05 15:48:10Z".
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* milspec();

    /**
     *  Print an milspec-style timestamp using a CommonEra date and time.
     *
     *  Example: "2005-Jul-05 15:48:10Z".
     *
     *  @param  ce      refers to a Common Era object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* milspec(const CommonEra& ce);

    /**
     *  Print an milspec-style timestamp using a LocalTime date and time.
     *
     *  Example: "2005-Jul-05 15:48:10T".
     *
     *  @param  lt      refers to a Local Time object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* milspec(const LocalTime& lt);

    /**
     *  Print an civilian-style timestamp using the current platform
     *  time.
     *
     *  Example: "2005-07-05 15:48:10 MDT".
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* civilian();

    /**
     *  Print an civilian-style timestamp using a CommonEra date and time.
     *
     *  Example: "2005-07-05 15:48:10 MDT".
     *
     *  @param  ce      refers to a Common Era object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* civilian(const CommonEra& ce);

    /**
     *  Print an civilian-style timestamp using a LocalTime date and time.
     *
     *  Example: "2005-07-05 15:48:10 MDT".
     *
     *  @param  lt      refers to a Local Time object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* civilian(const LocalTime& lt);

    /**
     *  Print an log-style timestamp using the current platform
     *  time.
     *
     *  Example: "2005-07-05T15:48:10.539047000Z".
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* log();

    /**
     *  Print an log-style timestamp using a CommonEra date and time.
     *
     *  Example: "2005-07-05T15:48:10.539047000Z".
     *
     *  @param  ce      refers to a Common Era object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* log(const CommonEra& ce);

    /**
     *  Print an log-style timestamp using a LocalTime date and time.
     *
     *  Example: "2005-07-05T15:48:10.539047000T".
     *
     *  @param  lt      refers to a Local Time object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* log(const LocalTime& lt);

    /**
     *  Print a high precision timestamp using the current platform time.
     *
     *  Example: "2005-07-05T15:48:10.123456789-00:00:00".
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* highprecision();

    /**
     *  Print a high precision timestamp using a CommonEra date and time.
     *
     *  Example: "2005-07-05T15:48:10.123456789-00:00:00".
     *
     *  @param  ce      refers to a Common Era object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* highprecision(const CommonEra& ce);

    /**
     *  Print a high precision timestamp using a LocalTime date and time.
     *
     *  Example: "2005-07-05T15:48:10.123456789-07:00:00".
     *
     *  @param  lt      refers to a Local Time object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* highprecision(const LocalTime& lt);

    /**
     *  Print an formal-style timestamp using the current platform
     *  time.
     *
     *  Example: "Tuesday, July 5, 2005, 3:48PM UTC".
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* formal();

    /**
     *  Print an formal-style timestamp using a CommonEra date and time.
     *
     *  Example: "Tuesday, July 5, 2005, 3:48PM UTC".
     *
     *  @param  ce      refers to a Common Era object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* formal(const CommonEra& ce);

    /**
     *  Print an formal-style timestamp using a LocalTime date and time.
     *
     *  Example: "Tuesday, July 5, 2005, 3:48PM MDT".
     *
     *  @param  lt      refers to a Local Time object.
     *
     *  @return a pointer to an internal buffer containing the timestamp.
     */
    virtual const char* formal(const LocalTime& lt);

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
     *  This is the internal buffer into which the timestamp is formatted.
     */
    char buffer[sizeof("WWWWWWWWW YYYYYYYYY-MM-DD hh:mm:ss.nnnnnnnnn +ooooooooo +dddd ZZZ-ZZZ")];

};

#include "com/diag/desperado/End.h"


#endif
