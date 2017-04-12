#ifndef _COM_DIAG_DESPERADO_CONSTANT_H_
#define _COM_DIAG_DESPERADO_CONSTANT_H_

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
 *  Declares the Constant class.
 *
 *  @see    Constant
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements some useful manifest constants in a common location.
 *  Where possible, the abbreviations are based on the recommendations
 *  of the US National Institute of Standards and Technology (NIST)
 *  and the Comite International des Poids et Mesures (CIPM). Note
 *  that the abbreviations are case sensitive. For example "m" is
 *  "milli" whereas "M" is "mega". The recommended abbreviations
 *  take precedence over any case-sensitive symbol naming conventions.
 *
 *  @see    B. N. Taylor, <I>Guide for the Use of the Inernational
 *          System of Units (SI)</I>, Special Publication 811, NIST,
 *          1995
 *
 *  @see    B. N. Taylor, ed., <I>The International System of Units
 *          (SI)</I>, Special Publication 330, NIST, 2001
 *
 *  @see    B. N. Taylor, <I>Metric System of Measurements: Interpretation
 *          of the International System of Units for the United States;
 *          Notice</I>, Part II, NIST, July 1998
 *
 *  @see    <I>The NIST Reference on Constants, Units, and Uncertainty</I>,
 *          NIST
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
struct Constant {

    /**
     *  This is the number of milliseconds in a second.
     */
    static const uint32_t ms_per_s = 1000;

    /**
     *  This is the number of microseconds in a second.
     */
    static const uint32_t us_per_s = 1000 * ms_per_s;

    /**
     *  This is the number of nanoseconds in a second.
     */
    static const uint32_t ns_per_s = 1000 * us_per_s;

    /**
     *  This is the number of nanoseconds in a millisecond.
     */
    static const uint32_t ns_per_ms = ns_per_s / ms_per_s;

    /**
     *  This is the number of seconds in a minute.
     */
    static const uint32_t s_per_min = 60;

    /**
     *  This is the number of minutes in an hour.
     */
    static const uint32_t min_per_h = 60;

    /**
     *  This is the number of seconds in an hour.
     */
    static const uint32_t s_per_h = s_per_min * min_per_h;

    /**
     *  This is the number of hours per day.
     */
    static const uint32_t h_per_d = 24;

    /**
     *  This is the number of seconds in a day.
     */
    static const uint32_t s_per_d = s_per_h * h_per_d;

    /**
     *  This is the number of months in a year.
     */
    static const uint32_t months_per_year = 12;

    /**
     *  This is the number of bits in an octet.
     */
    static const size_t bits_per_octet = 8;

};

#include "com/diag/desperado/End.h"


#endif
