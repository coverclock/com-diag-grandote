#ifndef _COM_DIAG_DESPERADO_METER_H_
#define _COM_DIAG_DESPERADO_METER_H_

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
 *  Declares the Meter class.
 *
 *  @see    Meter
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a general mechanism for maintaining the running minimum,
 *  maximum, and mean of a ratio measurement, for example, a rate
 *  like cycles per second, kilometers per hour, or furlongs per
 *  fortnight. The running statistics are maintained using a sliding
 *  window in which the accumulated numerator and denominator are scaled
 *  by bit shifting so that they cannot overflow. This has the usually
 *  desirable side effect of weighting the most recent measurements
 *  more heavily than earlier measurements.
 *
 *  Thanks to John Meiners for suggesting this strategy.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Meter : public Object {

public:

    /**
     *  Constructor.
     */
    explicit Meter();

    /**
     *  Destructor.
     */
    virtual ~Meter();

    /**
     *  Resets the object back to its just constructed state.
     */
    virtual void reset();

    /**
     *  Accumulate a sample by updating all the accumulators
     *  in this object, scaling if necessary to avoid overflow.
     *
     *  @param  n       is the measurement to be added to the
     *                  numerator accumulator.
     *
     *  @param  d       is the measurement to be added to the
     *                  denominator accumulator.
     *
     *  @param  round   specifies how the result should be rounded:
     *                  a number less than zero causes the result to
     *                  be truncated (floor); a zero causes the result
     *                  to be rounded up if the fraction is 0.5 or
     *                  above; a number greater than zero causes the
     *                  result to be rounded up if the fraction is
     *                  non-zero (ceiling).
     *
     *  @return         the ratio of the numerator and denominator.
     */
    virtual size_t sample(size_t n, size_t d = 1, int round = -1);

    /**
     *  Compute the mean value of the ratio.
     *
     *  @param  round   specifies how the result should be rounded:
     *                  a number less than zero causes the result to
     *                  be truncated (floor); a zero causes the result
     *                  to be rounded up if the fraction is 0.5 or
     *                  above; a number greater than zero causes the
     *                  result to be rounded up if the fraction is
     *                  non-zero (ceiling).
     *
     *  @return         the mean value of the ratio.
     */
    virtual size_t mean(int round = -1) const;

    /**
     *  Compute a ratio given a numerator and a denominator.
     *
     *  @param  n       is the measurement to be added to the
     *                  numerator accumulator.
     *
     *  @param  d       is the measurement to be added to the
     *                  denominator accumulator.
     *
     *  @param  round   specifies how the result should be rounded:
     *                  a number less than zero causes the result to
     *                  be truncated (floor); a zero causes the result
     *                  to be rounded up if the fraction is 0.5 or
     *                  above; a number greater than zero causes the
     *                  result to be rounded up if the fraction is
     *                  non-zero (ceiling).
     *
     *  @return         the ratio of the numerator and denominator.
     */
    virtual size_t ratio(size_t n, size_t d, int round = -1) const;

    /**
     *  Returns the scaled count of the number of samples.
     *
     *  @return         the scaled count of the number of samples.
     */
    size_t getSampled() const;

    /**
     *  Returns the count of the number of times the numerator and
     *  denominator accumulators have been shifted right to avoid
     *  one of them overflowing.
     *
     *  @return         the number of times the numerator, denominator,
     *                  and sample count accumulators have been shifted
     *                  right to avoid one of them overflowing.
     */
    size_t getScaled() const;

    /**
     *  Return the value of the numerator accumulator.
     *
     *  @return         the value of the numerator accumulator.
     */
    size_t getNumerator() const;

    /**
     *  Return the value of the denominator accumulator.
     *
     *  @return         the value of the denominator accumulator.
     */
    size_t getDenominator() const;

    /**
     *  Return the minimum value of the ratio.
     *
     *  @return         the minimum value of the ratio.
     */
    size_t getMinimum() const;

    /**
     *  Return the maximum value of the ratio.
     *
     *  @return         the maximum value of the ratio.
     */
    size_t getMaximum() const;

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

public:

    /**
     *  This is the sample count.
     */
    size_t sampled;

    /**
     *  This is the number of times the accumulators have been scaled.
     */
    size_t scaled;

    /**
     *  This is the numerator accumulator.
     */
    size_t numerator;

    /**
     *  This is the denominator accumulator.
     */
    size_t denominator;

    /**
     *  This is the minimum instantaneous ratio.
     */
    size_t minimum;

    /**
     *  This is the maximum instantaneous ratio.
     */
    size_t maximum;

};


//
//  Return the sample count.
//
inline size_t Meter::getSampled() const {
    return this->sampled;
}


//
//  Return the scaling count.
//
inline size_t Meter::getScaled() const {
    return this->scaled;
}


//
//  Return the numerator accumulator.
//
inline size_t Meter::getNumerator() const {
    return this->numerator;
}


//
//  Return the denominator accumulator.
//
inline size_t Meter::getDenominator() const {
    return this->denominator;
}


//
//  Return the minimum.
//
inline size_t Meter::getMinimum() const {
    return this->minimum;
}


//
//  Return the maximum.
//
inline size_t Meter::getMaximum() const {
    return this->maximum;
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Meter unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestMeter(void);
#endif


#endif
