#ifndef _COM_DIAG_DESPERADO_RATIO_H_
#define _COM_DIAG_DESPERADO_RATIO_H_

/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2009-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Declares the Ratio class.
 */


#include "com/diag/desperado/Begin.h"

/**
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Ratio {

public:

	typedef int Type;

    Ratio(Type nn = 0, Type dd = 1)
    : nu(nn)
    , de(dd)
    {
        normalize();
    }

    ~Ratio() {}

    Ratio(const Ratio & that) {
        this->nu = that.nu;
        this->de = that.de;
    }

    Type numerator() const { return nu; }

    Type denominator() const { return de; }

    Ratio * numerator(Type nn) { nu = nn; return this; }

    Ratio * denominator(Type dd) { de = dd; return this; }

    Ratio * normalize();

    operator double() const {
        double temp = this->nu;
        temp /= this->de;
        return temp;
    }

    operator float() const {
        float temp = this->nu;
        temp /= this->de;
        return temp;
    }

    operator int() const {
        int temp = this->nu;
        temp /= this->de;
        return temp;
    }

    operator long() const {
        long temp = this->nu;
        temp /= this->de;
        return temp;
    }

    operator long long() const {
        long long temp = this->nu;
        temp /= this->de;
        return temp;
    }

    Ratio & operator = (const Ratio & that) {
        if (this != &that) {
            this->nu = that.nu;
            this->de = that.de;
        }
        return *normalize();
    }

    Ratio & operator += (const Ratio & that) {
        if (this->de == that.de) {
            this->nu += that.nu;
        } else {
            this->nu = (this->nu * that.de) + (that.nu * this->de);
            this->de = (this->de * that.de);
        }
        return *normalize();
    }

    Ratio & operator -= (const Ratio & that) {
        if (this->de == that.de) {
            this->nu -= that.nu;
        } else {
            this->nu = (this->nu * that.de) - (that.nu * this->de);
            this->de = (this->de * that.de);
        }
        return *normalize();
    }

    Ratio & operator *= (const Ratio & that) {
        this->nu *= that.nu;
        this->de *= that.de;
        return *normalize();
    }

    Ratio & operator /= (const Ratio & that) {
        this->nu *= that.de;
        this->de *= that.nu;
        return *normalize();
    }

    Ratio operator + (const Ratio &that) const {
        Ratio temp = *this;
        temp += that;
        return *(temp.normalize());
    }

    Ratio operator - (const Ratio &that) const {
        Ratio temp = *this;
        temp -= that;
        return *(temp.normalize());
    }

    Ratio operator * (const Ratio &that) const {
        Ratio temp = *this;
        temp *= that;
        return *(temp.normalize());
    }

    Ratio operator / (const Ratio &that) const {
        Ratio temp = *this;
        temp /= that;
        return *(temp.normalize());
    }

    bool operator == (const Ratio &that) const {
        if (this->de == that.de) {
            return this->nu == that.nu;
        } else {
            return (this->nu * that.de) == (that.nu * this->de);
        }
    }

    bool operator != (const Ratio &that) const {
        return !(*this == that);
    }

    friend bool operator > (const Ratio & a, const Ratio & b);
    friend bool operator >= (const Ratio & a, const Ratio & b);
    friend bool operator < (const Ratio & a, const Ratio & b);
    friend bool operator <= (const Ratio & a, const Ratio & b);

private:

	Type nu;
    Type de;

};

inline bool operator > (const Ratio & a, const Ratio & b) {
    if (a.denominator() == b.denominator()) {
        return a.numerator() > b.numerator();
    } else {
        return (a.numerator() * b.denominator()) > (b.numerator() * a.denominator());
    }
}

inline bool operator >= (const Ratio & a, const Ratio & b) {
    if (a.denominator() == b.denominator()) {
        return a.numerator() >= b.numerator();
    } else {
        return (a.numerator() * b.denominator()) >= (b.numerator() * a.denominator());
    }
}

inline bool operator < (const Ratio & a, const Ratio & b) {
    if (a.denominator() == b.denominator()) {
        return a.numerator() < b.numerator();
    } else {
        return (a.numerator() * b.denominator()) < (b.numerator() * a.denominator());
    }
}

inline bool operator<= (const Ratio & a, const Ratio & b) {
    if (a.denominator() == b.denominator()) {
        return a.numerator() <= b.numerator();
    } else {
        return (a.numerator() * b.denominator()) <= (b.numerator() * a.denominator());
    }
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Ratio unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestRatio(void);
#endif


#endif
