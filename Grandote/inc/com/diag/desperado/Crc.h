#ifndef _COM_DIAG_DESPERADO_CRC_H_
#define _COM_DIAG_DESPERADO_CRC_H_

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
 *  Declares the Crc class.
 *
 *  @see    Crc
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
extern "C" {
#include "com/diag/desperado/crcmodel.h"
}
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Generates sixteen bit and thirty-two bit Cyclic Redundancy Check
 *  generators whose algorithms are based on the public domain Rocksoft(tm)
 *  Model CRC Algorithm by Ross Williams. Rocksoft is a trademark of
 *  Rocksoft Pty Ltd, Adelaide, Australia. The type of the CRC to be
 *  generated must be a sixteen bit (e.g. uint16_t) or a thirty-two bit
 *  (e.g. uint32_t) integral data type.
 *
 *  @see    R. Williams, <I>A Painless Guide to CRC Error Detection
 *          Algorithms</I>, ftp.adelaide.edu.au/pub/rocksoft,
 *          Rocksoft Pty Ltd, Adelaide, Australia, June 3, 1993
 *
 *  @author coverclock@diag.com (Chip Overclock),
 *          ross@guest.adelaide.edu.au (Ross Williams)
 */
template <typename _TYPE_>
class Crc : public Object {

public:

    /**
     *  Constructor.
     *
     *  @param  poly    is the polynomial describing the cyclic
     *                  redundancy check. The polynomial is typically
     *                  one bit wider than widthof(_TYPE_), the most
     *                  significant bit assumed to be one.
     *
     *  @param  refl    is true if the CRC is reflected, false otherwise.
     *
     *  @param  init    is the initial value of the partial result
     *                  before any data is processed.
     *
     *  @param  xoro    is the value with which the partial result
     *                  is exclusive-or-ed to get the CRC.
     */
    explicit Crc(
        _TYPE_ poly = 0,
        bool refl = false,
        _TYPE_ init = 0,
        _TYPE_ xoro = 0
    );

    /**
     *  Destructor.
     */
    virtual ~Crc();

    /**
     *  Initializes the provided CRC state variable.
     *
     *  @param  state   refers to the CRC state variable in which
     *                  the partial result of the computation is
     *                  maintained. This is not the CRC value.
     *
     *  @return         the current CRC value.
     */
    _TYPE_ operator() (_TYPE_& state) const;

    /**
     *  Accumulates the CRC computation across a block of data into
     *  the state variable.
     *
     *  @param  state   refers to the CRC state variable in which
     *                  the partial result of the computation is
     *                  maintained. This is not the CRC value.
     *
     *  @param  data    points to a block of data.
     *
     *  @param  size    is the size of the block of data in octets.
     *
     *  @return         the current CRC value.
     */
    _TYPE_ operator() (_TYPE_& state, const void* data, size_t size) const;

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
     *  This is true if the CRC is reflected, false otherwise.
     */
    bool reflected;

    /**
     *  This is the polynomial that describes the CRC. By definition
     *  there is an assumed 1 bit to the left of the highest order bit
     *  in the polynomial value.
     */
    _TYPE_ polynomial;

    /**
     *  This is the initial value of the CRC.
     */
    _TYPE_ initial;

    /**
     *  This is the value with which the partial result is exclusive
     *  or'ed to get the CRC.
     */
    _TYPE_ xorot;

    /**
     *  This is the CRC table.
     */
    _TYPE_ table[rangeof(uint8_t, size_t)];

};


//
//  Constructor.
//
template <typename _TYPE_>
Crc<_TYPE_>::Crc(_TYPE_ poly, bool refl, _TYPE_ init, _TYPE_ xoro) :
    reflected(refl),
    polynomial(poly),
    initial(init),
    xorot(xoro)
{
    cm_t cm;
    cm.cm_width = widthof(_TYPE_);
    cm.cm_poly = poly;
    cm.cm_refin = refl ? TRUE : FALSE;

    if (this->polynomial) {
        for (size_t ii = 0; countof(this->table) > ii; ++ii) {
            this->table[ii] = cm_tab(&cm, static_cast<int>(ii));
        }
    } else {
        for (size_t ii = 0; countof(this->table) > ii; ++ii) {
            this->table[ii] = 0;
        }
    }
}


//
//  Destructor.
//
template <typename _TYPE_>
Crc<_TYPE_>::~Crc() {
}


//
//  Functor to initialize state.
//
template <typename _TYPE_>
_TYPE_ Crc<_TYPE_>::operator() (_TYPE_& state) const {
    state = this->initial;
    return (state ^ this->xorot);
}


//
//  Functor.
//
template <typename _TYPE_>
_TYPE_ Crc<_TYPE_>::operator() (
    _TYPE_& state,
    const void* data,
    size_t size
) const {
    const uint8_t* pointer = static_cast<const uint8_t*>(data);
    if (!this->reflected) {
        while (size--) {
            state = this->table[
                    ((state >> (widthof(_TYPE_) - 8)) ^ (*(pointer++))) & 0xff
                ] ^ (state << 8);
        }
    } else {
        while (size--) {
            state = this->table[
                    (state ^ (*(pointer++))) & 0xff
                ] ^ (state >> 8);
        }
    }
    return (state ^ this->xorot);
}


//
//  Show this object on the output object.
//
template <typename _TYPE_>
void Crc<_TYPE_>::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    Output& out = printf.output();
    const char* sp = out.indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    const size_t width = widthof(_TYPE_);
    printf("%s widthof=%u\n", sp, width);
    printf("%s reflected=%u\n", sp, this->reflected);
    printf("%s polynomial=0x%x\n", sp, this->polynomial);
    printf("%s initial=0x%x\n", sp, this->initial);
    printf("%s xorot=0x%x\n", sp, this->xorot);
    printf("%s table(%p[%lu])[%lu]%s\n",
        sp, this->table, countof(this->table), sizeof(this->table),
        (0 < level) ? ":" : "");
    if (0 < level) {
        const char* format;
        unsigned int per;
        switch (width) {
        case widthof(uint16_t):
            format = "0x%04x";
            per = 8;
            break;
        case widthof(uint32_t):
            format = "0x%08x";
            per = 4;
            break;
        default:
            return;
        }
        out(sp);
        out("  ");
        for (size_t ii = 0; countof(this->table) > ii; ++ii) {
            printf(format, this->table[ii]);
            if (255 != ii) {
                out(',');
            }
            if (0 == ((ii + 1) % per)) {
                out('\n');
                if (255 != ii) {
                    out(sp);
                    out("  ");
                }
            }
        }
    }
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Crc unit test.
 *
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestCrc(void);
#endif


#endif
