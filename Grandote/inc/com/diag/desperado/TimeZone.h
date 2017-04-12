#ifndef _COM_DIAG_DESPERADO_TIMEZONE_H_
#define _COM_DIAG_DESPERADO_TIMEZONE_H_

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
 *  Declares the TimeZone class.
 *
 *  @see    TimeZone
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Defines time zone offsets and implements conversions of
 *  these offsets into printable strings indicating a time zone.
 *  The names follow the standard NATO convention of hours offset
 *  east or west of the zero, or Greenwich, meridian. They
 *  also follow the convention that the International Date
 *  Line splits the twelveth time zone into two half zones,
 *  each of which has the same clock time, but twenty-four
 *  hours apart. Zulu indicates coordinated universal time
 *  (UTC), formerly known as Greenwich Mean Time (GMT). Juliet
 *  is not an actual zone, but is used to indicate Local
 *  Civilian Time (LCT), the actual time zone depending on
 *  local context.
 *
 *  @see    J. R. Stockton, <I>Date and Time Formats</I>,
 *          http://www.merlyn.demon.co.uk
 *
 *  @see    P. Chan et al., <I>The Java Class Libraries</I>, Second
 *          Edition, Volume 1, Addison-Wesley, 1998, pp. 1775-1776
 *
 *  @see    glibc-2.2.5/time/offtime.c
 *
 *  @see    http://www.timeanddate.com
 *
 *  @see    http://greenwichmeantime.com
 *
 *  @see    http://wikipedia.org/Time_zone
 *
 *  @see    CommonEra
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class TimeZone : public Object {

public:

    /**
     *  This is the number of seconds per hour. For time zones
     *  calculations, it turns out to be useful for this to be
     *  signed.
     */
    static const int s_per_h = static_cast<int>(Constant::s_per_h);

    /**
     *  This is the number of seconds per minute. For time zones
     *  calculations, it turns out to be useful for this to be
     *  signed.
     */
    static const int s_per_min = static_cast<int>(Constant::s_per_min);

    /**
     *  This is the number of seconds per day. For time zones
     *  calculations, it turns out to be useful for this to be
     *  signed.
     */
    static const int s_per_d = static_cast<int>(Constant::s_per_d);

    /**
     *  Defines time zone offsets as NATO-comformant suffixes.
     */
    struct Suffix {
        /**
         *  Defines time zone offsets as NATO-comformant suffixes.
         */
        enum Offset {
            YANKEE      =   'Y',
            XRAY        =   'X',
            WHISKY      =   'W',
            VICTOR      =   'V',
            UNIFORM     =   'U',
            TANGO       =   'T',
            SIERRA      =   'S',
            ROMEO       =   'R',
            QUEBEC      =   'Q',
            PAPA        =   'P',
            OSCAR       =   'O',
            NOVEMBER    =   'N',
            ZULU        =   'Z',
            ALPHA       =   'A',
            BRAVO       =   'B',
            CHARLIE     =   'C',
            DELTA       =   'D',
            ECHO        =   'E',
            FOXTROT     =   'F',
            GOLF        =   'G',
            HOTEL       =   'H',
            INDIA       =   'I',
            JULIET      =   'J',
            KILO        =   'K',
            LIMA        =   'L',
            MIKE        =   'M'
        };
    };

    /**
     *  Defines time zone offsets in hours.
     */
    struct Hours {
        /**
         *  Defines time zone offsets in hours.
         */
        enum Offset {
            YANKEE      =   -12,
            MINIMUM     =   YANKEE,     // Minimum time zone value
            WEST        =   YANKEE,     // International Date Line West
            XRAY        =   -11,        // XRAY
            MIT         =   XRAY,       // Midway Islands Time
            WHISKY      =   -10,        // WHISKY
            HST         =   WHISKY,     // Hawaii Standard Time
            VICTOR      =   -9,         // VICTOR
            AST         =   VICTOR,     // Alaska Standard Time
            UNIFORM     =   -8,         // UNIFORM
            PST         =   UNIFORM,    // Pacific Standard Time
            TANGO       =   -7,         // TANGO
            MST         =   TANGO,      // Mountain Standard Time
            PNT         =   TANGO,      // Phoenix Standard TIme
            SIERRA      =   -6,         // SIERRA
            CST         =   SIERRA,     // Central Standard Time
            ROMEO       =   -5,         // ROMEO
            EST         =   ROMEO,      // Eastern Standard Time    
            IET         =   ROMEO,      // Indiana Eastern Standard Time
            QUEBEC      =   -4,         // QUEBEC
            PRT         =   QUEBEC,     // Puerto Rico/US Virgin Islands Time
//          CNT         =   -3.5,       // Canada Newfoundland Time
            PAPA        =   -3,         // PAPA
            AGT         =   PAPA,       // Argentina Standard Time
            BET         =   PAPA,       // Brazil Eastern Time
            OSCAR       =   -2,         // OSCAR
            NOVEMBER    =   -1,         // NOVEMBER
            CAT         =   NOVEMBER,   // Central African Time
            ZULU        =   0,          // ZULU
            UTC         =   ZULU,       // Universal Coordinated Time
            GMT         =   ZULU,       // Greenwich Mean Time
            ALPHA       =   1,          // ALPHA
            ECT         =   ALPHA,      // European Central Time
            EET         =   ALPHA,      // Eastern European Time
            BRAVO       =   2,          // BRAVO
            ART         =   BRAVO,      // (Arabic) Egypt Standard Time
            CHARLIE     =   3,          // CHARLIE
            EAT         =   CHARLIE,    // Eastern African Time
//          MET         =   3.5,        // Middle East Time
            DELTA       =   4,          // DELTA
            NET         =   DELTA,      // Near East Time
            ECHO        =   5,          // ECHO
            PLT         =   ECHO,       // Pakistan Lahore Time
//          IST         =   5.5,        // India Standard Time
            FOXTROT     =   6,          // FOXTROT
            BST         =   FOXTROT,    // Bangladesh Standard TIme
            GOLF        =   7,          // GOLF
            VST         =   GOLF,       // Vietnam Standard Time
            HOTEL       =   8,          // HOTEL
            CTT         =   HOTEL,      // China Taiwan Time
            INDIA       =   9,          // INDIA (the letter not the country)
            JST         =   INDIA,      // Japan Standard Time
//          ACT         =   9.5,        // Australia Central Time
//          JULIET      =   n/a,        // JULIET
//          LCT         =   JULIET,     // Local Civilian Time
            KILO        =   10,         // KILO
            AET         =   KILO,       // Australia Eastern Time
            LIMA        =   11,         // LIMA
            SST         =   LIMA,       // Solomon Standard Time
            MIKE        =   12,         // MIKE
            NST         =   MIKE,       // New Zealand Standard Time
            EAST        =   MIKE,       // International Date Line East
            MAXIMUM     =   MIKE        // Maximum time zone value
        };
    };

    /**
     *  Defines time zone offsets in seconds.
     */
    struct Seconds {
        /**
         *  Defines time zone offsets in seconds.
         */
        enum Offset {
            YANKEE      =   (Hours::YANKEE * s_per_h),
            MINIMUM     =   (Hours::MINIMUM * s_per_h),
            WEST        =   (Hours::WEST * s_per_h),
            XRAY        =   (Hours::XRAY * s_per_h),
            MIT         =   (Hours::MIT * s_per_h),
            WHISKY      =   (Hours::WHISKY * s_per_h),
            HST         =   (Hours::HST * s_per_h),
            VICTOR      =   (Hours::VICTOR * s_per_h),
            AST         =   (Hours::AST * s_per_h),
            UNIFORM     =   (Hours::UNIFORM * s_per_h),
            PST         =   (Hours::PST * s_per_h),
            TANGO       =   (Hours::TANGO * s_per_h),
            MST         =   (Hours::MST * s_per_h),
            PNT         =   (Hours::PNT * s_per_h),
            SIERRA      =   (Hours::SIERRA * s_per_h),
            CST         =   (Hours::CST * s_per_h),
            ROMEO       =   (Hours::ROMEO * s_per_h),
            EST         =   (Hours::EST * s_per_h),
            IET         =   (Hours::IET * s_per_h),
            QUEBEC      =   (Hours::QUEBEC * s_per_h),
            PRT         =   (Hours::PRT * s_per_h),
            CNT         =   (Hours::QUEBEC * s_per_h) - (s_per_h / 2),
            PAPA        =   (Hours::PAPA * s_per_h),
            AGT         =   (Hours::AGT * s_per_h),
            BET         =   (Hours::BET * s_per_h),
            OSCAR       =   (Hours::OSCAR * s_per_h),
            NOVEMBER    =   (Hours::NOVEMBER * s_per_h),
            CAT         =   (Hours::CAT * s_per_h),
            ZULU        =   (Hours::ZULU * s_per_h),
            UTC         =   (Hours::UTC * s_per_h),
            GMT         =   (Hours::GMT * s_per_h),
            ALPHA       =   (Hours::ALPHA * s_per_h),
            ECT         =   (Hours::ECT * s_per_h),
            EET         =   (Hours::EET * s_per_h),
            BRAVO       =   (Hours::BRAVO * s_per_h),
            ART         =   (Hours::ART * s_per_h),
            CHARLIE     =   (Hours::CHARLIE * s_per_h),
            EAT         =   (Hours::EAT * s_per_h),
            MET         =   (Hours::CHARLIE * s_per_h) + (s_per_h / 2),
            DELTA       =   (Hours::DELTA * s_per_h),
            NET         =   (Hours::NET * s_per_h),
            ECHO        =   (Hours::ECHO * s_per_h),
            PLT         =   (Hours::PLT * s_per_h),
            IST         =   (Hours::ECHO * s_per_h) + (s_per_h / 2),
            FOXTROT     =   (Hours::FOXTROT * s_per_h),
            BST         =   (Hours::BST * s_per_h),
            GOLF        =   (Hours::GOLF * s_per_h),
            VST         =   (Hours::VST * s_per_h),
            HOTEL       =   (Hours::HOTEL * s_per_h),
            CTT         =   (Hours::CTT * s_per_h),
            INDIA       =   (Hours::INDIA * s_per_h),
            JST         =   (Hours::JST * s_per_h),
            ACT         =   (Hours::INDIA * s_per_h) + (s_per_h / 2),
//          JULIET      =   (Hours::n/a * s_per_h),
//          LCT         =   (Hours::JULIET * s_per_h),
            KILO        =   (Hours::KILO * s_per_h),
            AET         =   (Hours::AET * s_per_h),
            LIMA        =   (Hours::LIMA * s_per_h),
            SST         =   (Hours::SST * s_per_h),
            MIKE        =   (Hours::MIKE * s_per_h),
            NST         =   (Hours::NST * s_per_h),
            EAST        =   (Hours::EAST * s_per_h),
            MAXIMUM     =   (Hours::MAXIMUM * s_per_h)
        };
    };

    /**
     *  Constructor.
     */
    explicit TimeZone();

    /**
     *  Destructor.
     */
    virtual ~TimeZone();

    /**
     *  Returns true if the offset represents a valid time zone.
     *  No judgement can be made whether the time zone is actually
     *  correct, merely that its value is in range.
     *
     *  @return true if the offset is valid, false otherwise.
     */
    bool isValid(int32_t ot) const;

    /**
     *  Normalizes a time zone offset in seconds to fall
     *  between the required MINIMUM and MAXIMUM inclusive.
     *
     *  @param  ot      is the offset in seconds from UTC.
     *
     *  @return a normalized offset in seconds from UTC.
     */
    int32_t normalize(int32_t ot) const;

    /**
     *  Returns the milspec time zone name given an offset
     *  in seconds from UTC.
     *
     *  @param  ot      is the offset in seconds from UTC.
     *
     *  @return milspec time zone name if it exists,
     *          a string indicating JULIET (a.k.a.
     *          Local Civilian Time) if it does not.
     */
    const char* milspec(int32_t ot) const;

    /**
     *  Returns the civilian time zone name given an offset
     *  in seconds from UTC.
     *
     *  @param  ot      is the offset in seconds from UTC.
     *
     *  @return civilian time zone name if it exists,
     *          a string indicating LCT (a.k.a.
     *          Local Civilian Time) if it does not.
     */
    const char* civilian(int32_t ot) const;

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

};


//
//  Constructor.
//
inline TimeZone::TimeZone() {
}


//
//  Return true if the offset is valid, false otherwise.
//
inline bool TimeZone::isValid(int32_t ot) const {
    return ((Seconds::MINIMUM <= ot) && (ot <= Seconds::MAXIMUM));
}


//
//  Normalize a time zone offset in seconds. This is a little more
//  complicated than one might think because there are in fact
//  twenty-five, not twenty-four, time zones. The time zone twelve
//  hours off UTC is in fact two half-sized time zones on either side
//  of the International Date Line. Further complications include
//  seconds east of UTC are negative, seconds west of UTC are positive.
//
inline int32_t TimeZone::normalize(int32_t ot) const {
    if (Seconds::MINIMUM > ot) {
        ot %= s_per_d;
        if (Seconds::MINIMUM > ot) {
            ot += s_per_d;
        }
    }
    else if (Seconds::MAXIMUM < ot) {
        ot %= s_per_d;
        if (Seconds::MAXIMUM < ot) {
            ot -= s_per_d;
        }
    }
    return ot;
}

#include "com/diag/desperado/End.h"


#endif
