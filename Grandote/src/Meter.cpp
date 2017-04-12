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
 *  Implements the Meter class.
 *
 *  @see    Meter
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Meter.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


static const size_t limit = ~(static_cast<size_t>(0));


//
//  Constructor.
//
Meter::Meter() :
    sampled(0),
    scaled(0),
    numerator(0),
    denominator(0),
    minimum(0),
    maximum(0)
{
}


//
//  Destructor.
//
Meter::~Meter() {
}



//
//  Reset the meter back to the initial conditions.
//
void Meter::reset() {
    this->sampled = 0;
    this->scaled = 0;
    this->numerator = 0;
    this->denominator = 0;
    this->minimum = 0;
    this->maximum = 0;
}


//
//  Accumulate a sample.
//
size_t Meter::sample(size_t n, size_t d, int round) {
    size_t ratio = this->ratio(n, d, round);
   
    if (0 == this->sampled) {
        this->minimum = ratio;
        this->maximum = ratio;
    } else {
        if (ratio < this->minimum) {
            this->minimum = ratio;
        }
        if (ratio > this->maximum) {
            this->maximum = ratio;
        }
    }

    size_t ln = limit - n;
    size_t ld = limit - d;

    while (
            (ln < this->numerator) ||
            (ld < this->denominator) ||
            (limit == this->sampled)
    ) {
        this->numerator >>= 1;
        this->denominator >>= 1;
        this->sampled >>= 1;
        if (limit > this->scaled) {
            ++this->scaled;
        }
    }

    this->numerator += n;
    this->denominator += d;
    ++this->sampled;

    return ratio;
}


//
//  Compute the mean.
//
size_t Meter::mean(int round) const {
    return this->ratio(this->numerator, this->denominator, round);
}


//
//  Compute a ratio given a numerator and denominator.
//
size_t Meter::ratio(size_t n, size_t d, int round) const {
    size_t result;
   
    if (0 == d) {
        result = limit;
    } else {
        result = n / d;
        if ((0 <= round) && (limit > result)) {
            size_t remainder = n % d;
            if (0 == round) {
                if ((limit > d) && (((d + 1) / 2) <= remainder)) {
                    ++result;
                }
            } else if (0 < round) {
                if (0 < remainder) {
                    ++result;
                }
            }
        }
    }

    return result;
}


//
//  Show this object on the output object.
//
void Meter::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s sampled=%u\n", sp, this->sampled);
    printf("%s scaled=%u\n", sp, this->scaled);
    printf("%s numerator=%u\n", sp, this->numerator);
    printf("%s denominator=%u\n", sp, this->denominator);
    printf("%s minimum=%u\n", sp, this->minimum);
    printf("%s maximum=%u\n", sp, this->maximum);
}


#include "com/diag/desperado/End.h"
