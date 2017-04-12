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
 *  Implements the Counters class.
 *
 *  @see    Counters
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Counters.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//
//  Constructor.
//
Counters::Counters(
    size_t ncounters,
    Counter* vcounters,
    const char** vlabels
) :
    countersn(((0 == ncounters) || (0 == vcounters)) ? 0 : ncounters),
    countersv(((0 == ncounters) || (0 == vcounters)) ? 0 : vcounters),
    labelsv(  ((0 == ncounters) || (0 == vcounters)) ? 0 : vlabels  )
{
}


//
//  Destructor.
//
Counters::~Counters() {
}


//
//  Reset all counters to a value.
//
void Counters::reset(Counter value) {
    if (0 != this->countersv) {
        for (unsigned int index = 0; index < this->countersn; ++index) {
            this->countersv[index] = value;
        }
    }
}


//
//  Generate a usable label for a counter identified by its index.
//
const char* Counters::generate(unsigned int index, Label buffer) const {
    const char* result;

    if ((0 != this->labelsv) && (0 != this->labelsv[index])) {
        result = this->labelsv[index];
    } else {
        std::sprintf(static_cast<char*>(buffer), "N%010u", index);
        result = buffer;
    }

    return result;
}


//
// Apply a functor to all counters.
//
bool Counters::apply(Functor& functor) {
    Label label;

    for (unsigned int index = 0; index < this->countersn; ++index) {
        if (!functor(
            index,
            this->generate(index, label),
            this->countersv[index]
        )){
            return false;
        }
    }

    return true;
}


//
// Format a counter for output.
//
const char* Counters::format(unsigned int index, Label buffer) const {
    if (0 < this->countersv[index]) {
        ::snprintf(buffer, sizeof(Label), "%11d", this->countersv[index]);
    } else {
        std::memset(buffer, ' ', sizeof(Label) - 1);
        buffer[sizeof(Label) - 2] = '.';
        buffer[sizeof(Label) - 1] = '\0';
    }
    return buffer;
}


//
//  Show this object on the output object.
//
void Counters::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);

    if (0 == level) {
        char component[sizeof(__FILE__)];
        printf("%s%s(%p)[%lu]:\n",
            sp, pl.component(__FILE__, component, sizeof(component)),
            this, sizeof(*this));
        printf("%s countersn=%lu\n", sp, this->countersn);
        printf("%s countersv=%p\n", sp, this->countersv);
        printf("%s labelsv=%p\n", sp, this->countersv);
        return;
    }

    if (0 == this->countersn) {
        return;
    }

    //
    //  Print labels.
    //

    Label label[5];
    size_t remaining = (0 != this->labelsv) ? this->countersn : 0;
    unsigned int index = 0;

    while (remaining > 0) {
    
        if (remaining >= 5) {

            printf(
                "%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->generate(index + 0, label[0]),
                this->generate(index + 1, label[1]),
                this->generate(index + 2, label[2]),
                this->generate(index + 3, label[3]),
                this->generate(index + 4, label[4]),
                index + 0, index + 4);

            index += 5;
            remaining -= 5;
    
        } else if (remaining >= 4) {

            printf(
                "%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->generate(index + 0, label[0]),
                this->generate(index + 1, label[1]),
                this->generate(index + 2, label[2]),
                this->generate(index + 3, label[3]),
                "",
                index + 0, index + 3);

            index += 4;
            remaining -= 4;
        
        } else if (remaining >= 3) {

            printf(
                "%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->generate(index + 0, label[0]),
                this->generate(index + 1, label[1]),
                this->generate(index + 2, label[2]),
                "",
                "",
                index + 0, index + 2);

            index += 3;
            remaining -= 3;
        
        } else if (remaining == 2) {
    
            printf(
                "%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->generate(index + 0, label[0]),
                this->generate(index + 1, label[1]),
                "",
                "",
                "",
                index + 0, index + 1);

            index += 2;
            remaining -= 2;
        
        } else {

            printf(
                "%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->generate(index + 0, label[0]),
                "",
                "",
                "",
                "",
                index + 0, index + 0);

            index += 1;
            remaining -= 1;
        
        }

    }

    //
    //  Print values.
    //

    remaining = this->countersn;
    index = 0;
    
    while (remaining > 0) {

        if (remaining >= 5) {

            printf("%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->format(index + 0, label[0]),
                this->format(index + 1, label[1]),
                this->format(index + 2, label[2]),
                this->format(index + 3, label[3]),
                this->format(index + 4, label[4]),
                index + 0, index + 4);

            index += 5;
            remaining -= 5;

        } else if (remaining >= 4) {

            printf("%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->format(index + 0, label[0]),
                this->format(index + 1, label[1]),
                this->format(index + 2, label[2]),
                this->format(index + 3, label[3]),
                "",
                index + 0, index + 3);

            index += 4;
            remaining -= 4;
    
        } else if (remaining >= 3) {

            printf("%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->format(index + 0, label[0]),
                this->format(index + 1, label[1]),
                this->format(index + 2, label[2]),
                "",
                "",
                index + 0, index + 2);

            index += 3;
            remaining -= 3;
    
        } else if (remaining == 2) {

            printf("%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->format(index + 0, label[0]),
                this->format(index + 1, label[1]),
                "",
                "",
                "",
                index + 0, index + 1);

            index += 2;
            remaining -= 2;
    
        } else {

            printf("%s%11.11s %11.11s %11.11s %11.11s %11.11s [%d..%d]\n",
                sp,
                this->format(index + 0, label[0]),
                "",
                "",
                "",
                "",
                index + 0, index + 0);

            index += 1;
            remaining -= 1;
    
        }
    
    }

}



//
//  Virtual destructor for Functor.
//
Counters::Functor::~Functor() {
}


#include "com/diag/desperado/End.h"
