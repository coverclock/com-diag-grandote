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
 *  Implements the Transliterator class.
 *
 *  @see    Transliterator
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Transliterator.h"
#include "com/diag/desperado/BufferInput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
Transliterator::Transliterator(uint8_t* st, unsigned int co) :
    stack(st),
    sp(0),
    spp(0),
    contexts(co),
    errors(0)
{
    for (int ss = S_FIRST; S_LAST >= ss; ++ss) {
        this->enter(
            static_cast<State>(ss),
            static_cast<Stimulus>(I_FIRST),
            static_cast<Stimulus>(I_LAST),
            A_INITIAL
        );
    }
}


//
//  Destructor.
//
Transliterator::~Transliterator() {
}


//
//  Transliterate a string.
//
char* Transliterator::next(char* from, char* to, size_t size) {
    BufferInput input(from);
    char* result = 0;

    Result rc = this->next(input, to, size);

    if (R_ERROR != rc) {

        while (R_EOL == rc) {
            rc = this->next(input, to, size);
        }

        size_t offset = input.getOffset();

        if (0 < offset) {
            result = from + offset;
        }

    }

    return result;
}


//
//  Transliterate a file.
//
Transliterator::Result Transliterator::next(
    Input& input,
    char *to,
    size_t size
) {
    Result rc = R_EOF;
    State state = S_INITIAL;
    int ch = input();
    char* t = to;
    size_t s = size;
    Stimulus stimulus;
    Action action;
    bool ready;

    this->prime(state);

    while (S_DONE != state) {

        if (EOF == ch) {
            if (size > s) {
                if (0 < s) {
                    *(t++) = '\0';
                    --s;
                } else {
                    ++(this->errors);
                    if (0 < size) {
                        to[size - 1] = '\0';
                    }
                }
            }
            break;
        }

        stimulus = static_cast<Stimulus>(ch);
        action = static_cast<Action>(table[state][stimulus]);
        ready = true;

        switch (action) {

        case A_SKIP:
            break;

        case A_TRAILING:
            state = S_WHITE;
            break;

        case A_STRING:
            this->append(stimulus, t, s);
            state = S_SOME;
            break;

        case A_CHARACTER:
            this->append(stimulus, t, s);
            this->insert('\0', t, s);
            rc = R_MORE;
            ready = false;
            state = S_DONE;
            break;

        case A_QUOTE:
            this->append(stimulus, t, s);
            this->poke((S_NONE == this->peek()) ? S_SOME : this->peek());
            break;

        case A_LITERAL:
            this->append(stimulus, t, s);
            this->poke((S_NONE == this->peek()) ? S_SOME : this->peek());
            state = this->pop();
            break;

        case A_ESCAPED:
            this->append('\\', t, s);
            this->append(stimulus, t, s);
            this->poke((S_NONE == this->peek()) ? S_SOME : this->peek());
            state=this->pop();
            break;

        case A_SINGLE:
            this->push(state);
            state = S_SINGLE;
            break;

        case A_DOUBLE:
            this->push(state);
            state = S_DOUBLE;
            break;

        case A_UNQUOTE:
            this->poke((S_NONE == this->peek()) ? S_SOME : this->peek());
            state = this->pop();
            break;

        case A_THIS:
            if (I_EOF == input(ch)) {
                ++(this->errors);
            }
            state = this->pop();
            break;

        case A_NEXT:
            state = this->pop();
            break;

        case A_ESCAPE:
            this->push(state);
            state = S_ESCAPE;
            break;

        case A_COMMENT:
            this->push(state);
            state = S_COMMENT;
            break;

        case A_IGNORE:
            this->push(state);
            state = S_IGNORE;
            break;

        case A_SPACE:
            ch = ' ';
            ready = false;
            break;

        case A_DONETHIS:
            if (I_EOF == input(ch)) {
                ++(this->errors);
            }
            rc = R_MORE;
            this->insert('\0', t, s);
            ready = false;
            state = S_DONE;
            break;

        case A_DONENEXT:
        case A_DONE:
            rc = R_EOL;
            ready = false;
            state = S_DONE;
            break;
        }

        if (ready) {
            ch = input();
        }
    }

    if ((0 < this->errors) && (R_MORE == rc)) {
        rc = R_ERROR;
    }

    return rc;
}


//
//  Show this object on the output object.
//
void Transliterator::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s stack=%p\n", sp, this->stack);
    printf("%s sp=%p\n", sp, this->sp);
    printf("%s spp=%p\n", sp, this->spp);
    printf("%s contexts=%u\n", sp, this->contexts);
    printf("%s errors=%u\n", sp, this->errors);
}


#include "com/diag/desperado/End.h"
