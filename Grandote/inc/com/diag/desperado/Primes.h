#ifndef _COM_DIAG_DESPERADO_PRIMES_H_
#define _COM_DIAG_DESPERADO_PRIMES_H_

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
 *  Declares the Primes class.
 */


#include <vector>
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Begin.h"

/**
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Primes {

public:

	typedef int Type;

    typedef std::vector<Type> Vector;

    typedef Vector::const_iterator Iterator;

    Primes(Type limit) { generate(limit); }

    ~Primes() {}

    size_t size() { return primes.size(); }

    Iterator begin() const { return primes.begin(); }

    Iterator end() const { return primes.end(); }

protected:

    static void generate(Type limit);

private:

    static Vector primes;
    
    static Type maximum;

};


#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Primes unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestPrimes(void);
#endif


#endif

