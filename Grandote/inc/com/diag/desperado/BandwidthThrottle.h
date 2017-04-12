#ifndef _COM_DIAG_DESPERADO_BANDWIDTHTHROTTLE_H_
#define _COM_DIAG_DESPERADO_BANDWIDTHTHROTTLE_H_

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
 *  Declares the BandwidthThrottle class.
 *
 *  @see    BandwidthThrottle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/types.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/CompoundThrottle.h"
#include "com/diag/desperado/Gcra.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"

/**
 *  This class implements a throttle based on the Generic Cell
 *  Rate Algorithm, but instead of rates measured in cells per
 *  second and time in microseconds (as per the ATM standards),
 *  it measures rates in bytes per second and time in nanoseconds.
 *  The bandwidth that may be specified is limited by the maximum of the
 *  size_t type and the resolution of the platform frequency. The former on
 *  most platforms limits bandwidth to about four billion bytes per second.
 *  The latter on Linux limits bandwidth to bytes million bytes per second.
 *  It is expected that most applications will use the form of commit() in
 *  which the number of events may be specified, for example a buffer size
 *  in bytes.
 *
 *  @see    Throttle
 *
 *  @see    Gcra
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class BandwidthThrottle : public CompoundThrottle {

public:

    /**
     *  Computes the GCRA increment or I for a peak contract.
     *
     *  @param  pbps        is the peak rate in bytes per second.
     *
     *  @param  jt          is the jitter tolerance in nanoseconds.
     *
     *  @return the resulting increment in ticks.
     */
    static ticks_t increment(uint32_t pbps, uint32_t jt);

    /**
     *  Computes the GCRA limit or L for a peak contract.
     *
     *  @param  pbps        is the peak rate in bytes per second.
     *
     *  @param  jt          is the jitter tolerance in nanoseconds.
     *
     *  @return the resulting limit in ticks.
     */
    static ticks_t limit(uint32_t pbps, uint32_t jt);

    /**
     *  Computes the GCRA increment or I for a sustained contract.
     *
     *  @param  pbps        is the peak rate in bytes per second.
     *
     *  @param  jt          is the jitter tolerance in nanoseconds.
     *
     *  @param  sbps        is the sustained rate in bytes per second.
     *
     *  @param  mbs         is the maximum burst size in bytes.
     *
     *  @return the resulting increment in ticks.
     */
    static ticks_t increment(uint32_t pbps, uint32_t jt, uint32_t sbps, uint32_t mbs);

    /**
     *  Computes the GCRA limit or L for a sustained contract.
     *
     *  @param  pbps        is the peak cell rate in bytes per second.
     *
     *  @param  jt          is the jitter tolerance in nanoseconds.
     *
     *  @param  sbps        is the sustained rate in bytes per second.
     *
     *  @param  mbs         is the maximum burst size in bytes.
     *
     *  @return the resulting limit in ticks.
     */
    static ticks_t limit(uint32_t pbps, uint32_t jt, uint32_t sbps, uint32_t mbs);

    /**
     *  Constructor.
     */
    explicit BandwidthThrottle();

    /**
     *  Constructor for a peak only contract.
     *
     *  @param  pbps        is the peak cell rate in bytes per second.
     *
     *  @param  jt          is the jitter tolerance in nanoseconds.
     */
    explicit BandwidthThrottle(uint32_t pbps, uint32_t jt);

    /**
     *  Constructor for peak and sustained contracts.
     *
     *  @param  pbps        is the peak cell rate in bytes per second.
     *
     *  @param  jt          is the jitter tolerance in nanoseconds.
     *
     *  @param  sbps        is the sustained rate in bytes per second.
     *
     *  @param  mbs         is the maximum burst size in bytes.
     */
    explicit BandwidthThrottle(uint32_t pbps, uint32_t jt, uint32_t sbps, uint32_t mbs);

    /**
     *  Destructor.
     */
    virtual ~BandwidthThrottle();

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
 *  Run the BandwidthThrottle unit test. This may take a while on a typical
 *  system.
 *
 *  @param  pVM         points to the Ficl VM in whose context this
 *                      function is being executed if being run from a Ficl
 *                      shell, null otherwise.
 *
 *  @param  pbps        is the Peak Rate in bytes per second.
 *
 *  @param  jt          is the Jitter Tolerance in nanoseconds.
 *
 *  @param  sbps        is the Sustainable Rate in bytes per second.
 *
 *  @param  mbs         is the Maximum Burst Size in cells.
 *
 *  @param  iterations  is the number of iterations for each sub-test.
 *
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestBandwidthThrottle(
    void* pVM,
    unsigned int pbps,
    unsigned int jt,
    unsigned int sbps,
    unsigned int mbs,
    unsigned int iterations
);
#endif


#endif
