#ifndef _COM_DIAG_DESPERADO_RAM_H_
#define _COM_DIAG_DESPERADO_RAM_H_

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
 *  Declares the Ram class.
 *
 *  @see    Ram
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/littleendian.h"
#include "com/diag/desperado/generics.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Generates a suite of memory diagnostics for a memory architecture with
 *  a width of any integral type, based on Michael Barr's memtest functions
 *  as described in his article on software-based memory testing.
 *
 *  @see    Barr, M., "Software-Based Memory Testing", <I>Embedded
 *          Systems Programming</I>, July 2000, pp. 28 - 40
 *
 *  @author coverclock@diag.com (Chip Overclock),
 *          Isaac Eyeson,
 *          Norm Petty,
 *          John Sloan
 */
template <typename _TYPE_>
class Ram : public Object {

public:

    /**
     *  Constructor.
     */
    explicit Ram();

    /**
     *  Destructor.
     */
    virtual ~Ram();

    /**
     *  Test the data bus wiring in a memory region by performing a walking
     *  1's test at a fixed address within that region. The address (and hence
     *  the memory region) is selected by the caller.
     *
     *  @param  address     points to the word used for the walking 1's
     *                      test in the memory region under test.
     *
     *  @return zero if the test succeeds, otherwise non-zero indicating
     *          the first pattern that failed.
     */
    virtual _TYPE_ data(
        volatile _TYPE_* address
    );

    /**
     *  Test the address bus wiring in a memory region by performing a walking
     *  1's test on the relevant bits of the address and checking for aliasing.
     *  This test will find single-bit address failures such as stuck-high,
     *  stuck-low, and shorted pins. The base address and size of the region
     *  are selected by the caller.
     *
     *  For best results, the selected base address should have enough LSB 0's
     *  to guarantee single address bit changes. For example, to test a 64-Kbyte
     *  region, select a base address on a 64-Kbyte boundary. Also, select the
     *  region size as a power-of-two if at all possible.
     *
     *  @param  baseAddress     points to the starting address for the
     *                          walking 1's test in the memory region under
     *                          test.
     *
     *  @param  nBytes          is the number of bytes in the memory region
     *                          under test.
     *
     *  @return NULL if the test succeeds. A non-zero result is the first
     *          address at which an aliasing problem was uncovered. By
     *          examining the contents of memory, it may be possible to
     *          gather additional information about the problem.
     */
    virtual volatile _TYPE_* address(
        volatile _TYPE_* baseAddress,
        size_t nBytes
    );

    /**
     *  Test the integrity of a physical memory device by performing an
     *  increment/decrement test over the entire region.  In the process
     *  every storage bit in the device is tested as a zero and a one. The
     *  base address and the size of the region are selected by the caller.
     *
     *  @param  baseAddress     points to the starting address for the
     *                          walking 1's test in the memory region under
     *                          test.
     *
     *  @param  nBytes          is the number of bytes in the memory region
     *                          under test.
     *
     * @return  NULL if the test succeeds. A non-zero result is the first
     *          address at which an incorrect value was read back. By
     *          examining the contents of memory, it may be possible to
     *          gather additional information about the problem.
     */
    virtual volatile _TYPE_* device(
        volatile _TYPE_* baseAddress,
        size_t nBytes
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
     *  This word contains the repeating bit pattern 1010.
     */
    _TYPE_ patternA;

    /**
     *  This word contains the repeating bit pattern 0101.
     */
    _TYPE_ pattern5;
};


//
// Constructor
//
template <typename _TYPE_>
Ram<_TYPE_>::Ram() {

    this->patternA = 0xAA;
    for (size_t ii = 1; ii < sizeof(_TYPE_); ++ii) {
        this->patternA = (this->patternA << 8) | 0xAA;
    }
    this->pattern5 = (this->patternA << 1) | 0x01;

}


//
//  Destructor.
//
template <typename _TYPE_>
Ram<_TYPE_>::~Ram() {
}


//
//  Show this object on the output object.
//
template <typename _TYPE_>
void Ram<_TYPE_>::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s widthof=%u\n", sp, widthof(_TYPE_));
    printf("%s widthof*=%u\n", sp, widthof(_TYPE_*));
    Dump dump;
    char hex[(sizeof(_TYPE_) * 2) + 1];
    printf("%s patternA=0x%s\n",
        sp, dump.string(&(this->patternA), sizeof(this->patternA), hex,
            littleendian()));
    printf("%s pattern5=0x%s\n",
        sp, dump.string(&(this->pattern5), sizeof(this->pattern5), hex,
            littleendian()));
}


/**********************************************************************
 *
 * Filename:    memtest.c.h
 * 
 * Description: General-purpose memory testing functions.
 *
 * Notes:       This software can be easily ported to systems with
 *              different data bus widths by redefining 'datum'.
 *
 * 
 * Copyright (c) 1998 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/


/**********************************************************************
 *
 * Function:    memTestDataBus()
 *
 * Description: Test the data bus wiring in a memory region by
 *              performing a walking 1's test at a fixed address
 *              within that region.  The address (and hence the
 *              memory region) is selected by the caller.
 *
 * Notes:       
 *
 * Returns:     0 if the test succeeds.  
 *              A non-zero result is the first pattern that failed.
 *
 **********************************************************************/
template <typename _TYPE_>
_TYPE_
Ram<_TYPE_>::data(volatile _TYPE_ * address)
{
    _TYPE_ pattern;


    /*
     * Perform a walking 1's test at the given address.
     */
    for (pattern = 1; pattern != 0; pattern <<= 1)
    {
        /*
         * Write the test pattern.
         */
        *address = pattern;

        /*
         * Read it back (immediately is okay for this test).
         */
        if (*address != pattern) 
        {
            return (pattern);
        }
    }

    return (0);

}   /* memTestDataBus() */


/**********************************************************************
 *
 * Function:    memTestAddressBus()
 *
 * Description: Test the address bus wiring in a memory region by
 *              performing a walking 1's test on the relevant bits
 *              of the address and checking for aliasing. This test
 *              will find single-bit address failures such as stuck
 *              -high, stuck-low, and shorted pins.  The base address
 *              and size of the region are selected by the caller.
 *
 * Notes:       For best results, the selected base address should
 *              have enough LSB 0's to guarantee single address bit
 *              changes.  For example, to test a 64-Kbyte region, 
 *              select a base address on a 64-Kbyte boundary.  Also, 
 *              select the region size as a power-of-two--if at all 
 *              possible.
 *
 * Returns:     NULL if the test succeeds.  
 *              A non-zero result is the first address at which an
 *              aliasing problem was uncovered.  By examining the
 *              contents of memory, it may be possible to gather
 *              additional information about the problem.
 *
 **********************************************************************/
template <typename _TYPE_>
volatile _TYPE_ * 
Ram<_TYPE_>::address(volatile _TYPE_ * baseAddress, size_t nBytes)
{
    size_t addressMask = (nBytes/sizeof(_TYPE_) - 1);
    size_t offset;
    size_t testOffset;

    _TYPE_ pattern     = this->patternA;
    _TYPE_ antipattern = this->pattern5;


    /*
     * Write the default pattern at each of the power-of-two offsets.
     */
    for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
    {
        baseAddress[offset] = pattern;
    }

    /* 
     * Check for address bits stuck high.
     */
    testOffset = 0;
    baseAddress[testOffset] = antipattern;

    for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
    {
        if (baseAddress[offset] != pattern)
        {
            return (&baseAddress[offset]);
        }
    }

    baseAddress[testOffset] = pattern;

    /*
     * Check for address bits stuck low or shorted.
     */
    for (testOffset = 1; (testOffset & addressMask) != 0; testOffset <<= 1)
    {
        baseAddress[testOffset] = antipattern;

        if (baseAddress[0] != pattern)
        {
            return (&baseAddress[testOffset]);
        }

        for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
        {
            if ((baseAddress[offset] != pattern) && (offset != testOffset))
            {
                return (&baseAddress[testOffset]);
            }
        }

        baseAddress[testOffset] = pattern;
    }

    return (NULL);

}   /* memTestAddressBus() */


/**********************************************************************
 *
 * Function:    memTestDevice()
 *
 * Description: Test the integrity of a physical memory device by
 *              performing an increment/decrement test over the
 *              entire region.  In the process every storage bit 
 *              in the device is tested as a zero and a one.  The
 *              base address and the size of the region are
 *              selected by the caller.
 *
 * Notes:       
 *
 * Returns:     NULL if the test succeeds.
 *
 *              A non-zero result is the first address at which an
 *              incorrect value was read back.  By examining the
 *              contents of memory, it may be possible to gather
 *              additional information about the problem.
 *
 **********************************************************************/
template <typename _TYPE_>
volatile _TYPE_ * 
Ram<_TYPE_>::device(volatile _TYPE_ * baseAddress, size_t nBytes)   
{
    size_t offset;
    size_t nWords = nBytes / sizeof(_TYPE_);

    _TYPE_ pattern;
    _TYPE_ antipattern;


    /*
     * Fill memory with a known pattern.
     */
    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
    {
        baseAddress[offset] = pattern;
    }

    /*
     * Check each location and invert it for the second pass.
     */
    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
    {
        if (baseAddress[offset] != pattern)
        {
            return (&baseAddress[offset]);
        }

        antipattern = ~pattern;
        baseAddress[offset] = antipattern;
    }

    /*
     * Check each location for the inverted pattern and zero it.
     */
    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
    {
        antipattern = ~pattern;
        if (baseAddress[offset] != antipattern)
        {
            return (&baseAddress[offset]);
        }

        baseAddress[offset] = 0;        
    }

    return (NULL);

}   /* memTestDevice() */

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Ram unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestRam(void);
#endif


#endif
