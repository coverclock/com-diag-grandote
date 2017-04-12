#ifndef _COM_DIAG_DESPERADO_CELLRATETHROTTLE_H_
#define _COM_DIAG_DESPERADO_CELLRATETHROTTLE_H_

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
 *  Declares the CellRateThrottle class.
 *
 *  @see    CellRateThrottle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/CompoundThrottle.h"
#include "com/diag/desperado/Gcra.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a throttle that enforces a traffic contract based on
 *  a virtual scheduler implementation of a Generic Cell Rate Algorithm
 *  (GCRA) as defined in the ATM Forum Traffic Management 4.1 specification.
 *  This throttle implements the cell rate traffic contracts using peak
 *  cell rate (PCR), cell delay variation tolerance (CDVT), sustainable
 *  cell rate (SCR), and maximum burst size (MBS), while the contract
 *  throttle implements a simpler approximation. This throttle uses
 *  sixty-four bit arithmetic which may yield more accurate results
 *  albeit at higher cost, while the contract throttle uses thirty-two
 *  bit arithmetic and sometimes has to fudge the traffic contract to
 *  do so.
 *
 *  Note that this implements a single traffic contract for either
 *  the peak or the sustainable cell rates. See the compound cell
 *  rate throttle for a throttle that implements both contracts
 *  simultaneously.
 *
 *  Although the traffic contract used by this throttle is specified
 *  in terms of ATM cells, it is trivially adaptable to throttling any
 *  event that can be expressed in terms of emissions per second. However,
 *  as with ATM traffic contracts, a rate slower than one emission per
 *  second cannot be specified.
 *
 *  As per ATM standards, the time units in which the traffic contract
 *  is specified is in microseconds. However, time of day and delay is
 *  specified in terms of platform ticks, eliminating any need for
 *  conversion to or from the platform values.
 *
 *  @see    Throttle
 *
 *  @see    Gcra
 *
 *  @see    CompoundThrottle
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class CellRateThrottle : public CompoundThrottle {

public:

    /**
     *  Computes the GCRA increment or I for a peak contract.
     *
     *  @param  pcr         is the peak cell rate in cells
     *                      per second.
     *
     *  @param  cdvt        is the cell delay variation tolerance
     *                      in microseconds.
     *
     *  @return the resulting increment in throttle ticks.
     */
    static ticks_t increment(uint32_t pcr, uint32_t cdvt);

    /**
     *  Computes the GCRA limit or L for a peak contract.
     *
     *  @param  pcr         is the peak cell rate in cells
     *                      per second.
     *
     *  @param  cdvt        is the cell delay variation tolerance
     *                      in microseconds.
     *
     *  @return the resulting limit in throttle ticks.
     */
    static ticks_t limit(uint32_t pcr, uint32_t cdvt);

    /**
     *  Computes the GCRA increment or I for a sustainable contract.
     *
     *  @param  pcr         is the peak cell rate in cells
     *                      per second.
     *
     *  @param  cdvt        is the cell delay variation tolerance
     *                      in microseconds.
     *
     *  @param  scr         is the sustainable cell rate in cells
     *                      per second.
     *
     *  @param  mbs         is the maximum burst size in cells.
     *
     *  @return the resulting increment in throttle ticks.
     */
    static ticks_t increment(uint32_t pcr, uint32_t cdvt, uint32_t scr, uint32_t mbs);

    /**
     *  Computes the GCRA limit or L for a sustainable contract.
     *
     *  @param  pcr         is the peak cell rate in cells
     *                      per second.
     *
     *  @param  cdvt        is the cell delay variation tolerance
     *                      in microseconds.
     *
     *  @param  scr         is the sustainable cell rate in cells
     *                      per second.
     *
     *  @param  mbs         is the maximum burst size in cells.
     *
     *  @return the resulting limit in throttle ticks.
     */
    static ticks_t limit(uint32_t pcr, uint32_t cdvt, uint32_t scr, uint32_t mbs);

    /**
     *  Constructor. Yields a null contract with I=(maximum) and
     *  L=0;
     */
    explicit CellRateThrottle();

    /**
     *  Constructor. Yields a peak contract for a constant cell
     *  rate given a Peak Cell Rate (PCR) in cells per second,
     *  and a Cell Delay Variation Tolerance (CDVT) (also known
     *  as jitter tolerance) in microseconds.
     *
     *  @param  pcr         is the peak cell rate in cells
     *                      per second.
     *
     *  @param  cdvt        is the cell delay variation tolerance
     *                      in microseconds.
     */
    explicit CellRateThrottle(uint32_t pcr, uint32_t cdvt);

    /**
     *  Constructor. Yields a sustainable contract for a variable cell
     *  rate given a Peak Cell Rate (PCR) in cells per second, a Cell
     *  Delay Variation Tolerance (CDVT) (also known as jitter
     *  tolerance) in microseconds, a Sustainable Cell Rate (SCR) in cells
     *  per second, and a Maximum Burst Size (MBS) in cells.
     *
     *  @param  pcr         is the peak cell rate in cells
     *                      per second.
     *
     *  @param  cdvt        is the cell delay variation tolerance
     *                      in microseconds.
     *
     *  @param  scr         is the sustainable cell rate in cells
     *                      per second.
     *
     *  @param  mbs         is the maximum burst size in cells.
     */
    explicit CellRateThrottle(uint32_t pcr, uint32_t cdvt, uint32_t scr, uint32_t mbs);

    /**
     *  Destructor.
     */
    virtual ~CellRateThrottle();

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

    Gcra peak;

    Gcra sustained;

};

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the CellRateThrottle unit test. This may take a while on a typical
 *  system.
 *
 *  @param  pVM         points to the Ficl VM in whose context this
 *                      function is being executed if being run from a Ficl
 *                      shell, null otherwise.
 *
 *  @param  pcr         is the Peak Cell Rate in cells per second.
 *
 *  @param  cdvt        is the Cell Delay Variation Tolerance in microseconds.
 *
 *  @param  scr         is the Sustainable Cell Rate in cells per second.
 *
 *  @param  mbs         is the Maximum Burst Size in cells.
 *
 *  @param  iterations  is the number of iterations for sach sub-test.
 *
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestCellRateThrottle(
    void* pVM,
    unsigned int pcr,
    unsigned int cdvt,
    unsigned int scr,
    unsigned int mbs,
    unsigned int iterations
);
#endif


#endif
