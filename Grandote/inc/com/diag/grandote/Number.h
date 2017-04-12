#ifndef _COM_DIAG_DESPERADO_NUMBER_H_
#define _COM_DIAG_DESPERADO_NUMBER_H_

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
 *  Declares the Number class.
 *
 *  @see    Number
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Generates a method to parse a character string and convert it into a
 *  binary integer of any integral type whose base is determined from its
 *  context, doing so in a slightly more robust manner than either atoi(3)
 *  or any of the strtol(3) variants.
 *
 *  A finite state machine syntax checks the number string and
 *  if it is valid returns it in a return-result reference parameter as
 *  an integer. Like strtol(), the radix is inferred from
 *  context by looking for a leading "0x" or "0X" (base 16), or
 *  "0" (base 8). Does stuff that strtol() and its kin do not:
 *  ignores leading and trailing white space, indicates how many characters
 *  were consumed by the parser; returns a partial result which may be
 *  useful; and returns an indication of whether the returned result is
 *  valid or not. The returned length is useful when parsing a numeric
 *  substring from a longer string.
 *
 *  The functor return true if the number is valid, false
 *  otherwise. Numbers may be invalid either because they contain
 *  syntax errors, or because the final result overflowed and will not
 *  fit in the result data type. For unsigned results, overflow means that
 *  bits were shifted out of the partial result and lost. For signed
 *  results, overflow can also mean bits were shifted into the
 *  sign bit. Syntax errors cannot be reliably distinguished from overflow
 *  errors in all circumtances, but generally if the returned length
 *  indicates that all characters in the string were consumed, yet false
 *  was returned, one may assume that an overflow error occurred. If the
 *  input string terminates with a character other than a nul or whitespace
 *  (for example, an equal sign or "="), it is considered a syntax error
 *  because the parser cannot reliably discern whether the terminating
 *  character is part of the application syntax or a syntax error.
 *  This makes the returned value less useful than it might otherwise be.
 *
 *  The template implementation allows a parser to be generated for
 *  any integral type, including for example pointers in the form
 *  of uintptr_t.
 *
 *  The grammer accepted by the parser for an integer is shown below
 *  in ISO EBNF. The productions imply what characters are consumed
 *  by the parser before it returns.
 *
 *  
 *  whitespace   = ( ' ' | TAB );<BR>
 *  sign         = ( '-' | '+' );<BR>
 *  x            = ( 'x' | 'X' );<BR>
 *  common       = ( '1' | '2' | '3' | '4' | '5' | '6' | '7' );<BR>
 *  octal        = ( '0' | common );<BR>
 *  predecimal   = ( common | '8' | '9' );<BR>
 *  decimal      = ( '0' | predecimal );<BR>
 *  lowercase    = ( 'a' | 'b' | 'c' | 'd' | 'e' | 'f' );<BR>
 *  uppercase    = ( 'A' | 'B' | 'C' | 'D' | 'E' | 'F' );<BR>
 *  hexadecimal  = ( decimal | lowercase | uppercase );<BR>
 *  prefix       = { whitespace }, [ sign ];<BR>
 *  suffix       = { whitespace }, [ NUL ];<BR>
 *  octals       = { octal };<BR>
 *  decimals     = predecimal, { decimal };<BR>
 *  hexadecimals = x, hexadecimal, { hexadecimal };<BR>
 *  nondecimals  = '0', [ hexadecimals | octals ];<BR>
 *  integer      = prefix, ( decimals | nondecimals ), suffix;<BR>
 *  
 *
 *  Examples:
 *
 *  
 *  "12345" returns true, result=12,345, length=6<BR>
 *  "X=12345" returns false, result=0, length=0<BR>
 *  "12345," returns false, result=12345, length=5<BR>
 *  "-12345" returns true, result=-12,345, length=7<BR>
 *  "0xc0edbabe" returns true, result=-1,058,161,986, length=11<BR>
 *  "0XDEAD" returns true, result=57,005, length=7<BR>
 *  "0177777" returns true, result=65,535, length=8<BR>
 *  "   1776   " returns true, result=1,776, length=11<BR>
 *  "0xc0gdbabe" returns false, result=192 length=4<BR>
 *  "0xc0 gdbabe" returns true, result=192, length=5<BR>
 *  "1.5" returns false, result=1, length=1<BR>
 *  
 *
 *  @see    ISO, <I>Extended BNF</I>, ISO/IEC 14977:1996(E)
 *
 *  @see    R. S. Scowen, <I>Extended BNF - A generic base
 *          standard</I>, Software Engineering Standards
 *          Symposium, 1993
 *
 *  @see    D. Crocker, <I>Augmented BNF for Syntax Specifications:
 *          ABNF</I>, RFC2234, November 1997
 *
 *  @see    atoi(3)
 *
 *  @see    strtol(3)
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
template <typename _TYPE_>
class Number : public Object {

public:

    /**
     *  Constructor.
     */
    explicit Number();

    /**
     *  Destructor.
     */
    virtual ~Number();

    /**
     *  Parses a character string and converts it into a integer.
     *
     *  @param  string          points to the character string containing
     *                          the possible number. The string itself
     *                          indicates the base of the number in context
     *                          using ANSI C rules.
     *
     *  @param  resultp         refers to the return-by-reference result
     *                          variable. If this method returns true,
     *                          the result contains the value. If this
     *                          method returns false, the result contains
     *                          the partial value collected so far.
     *
     *  @param  lengthp         refers to the return-by-reference length
     *                          variable that is set to the number of
     *                          characters scanned before the parser returned.
     *
     *  @param  size            is the maximum number of characters to be
     *                          scanned. This can be used to terminate the
     *                          scanning before the nul terminator is reached.
     *
     *  @return                 true if a valid number was parsed, false
     *                          otherwise.
     */
    virtual bool operator() (
        const char* string,
        _TYPE_& resultp,
        size_t& lengthp,
        size_t size = unsignedintmaxof(size_t)
    ) const;

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

};


//
// Constructor
//
template <typename _TYPE_>
inline Number<_TYPE_>::Number() {
}


//
//  Destructor.
//
template <typename _TYPE_>
Number<_TYPE_>::~Number() {
}


//
//  Parse a character string into an integer of the specified type.
//
template <typename _TYPE_>
bool Number<_TYPE_>::operator() (
    const char* string,
    _TYPE_& resultp,
    size_t& lengthp,
    size_t size
) const {
    enum State {
        STATE_LEADING,          // leading blanks or tabs
        STATE_SIGN,             // initial '-' or '+'
        STATE_DECIMAL,          // decimal digits 0..9
        STATE_NONDECIMAL,       // leading '0': octal or hex
        STATE_X,                // 'x' or 'X' after a leading '0': hex
        STATE_OCTAL,            // octal digits 0..7
        STATE_HEXADECIMAL,      // hex digits a..f or A..F
        STATE_TRAILING,         // trailing blanks or tabs
        STATE_END,              // complete result
        STATE_ERROR             // partial result
    };
    static const _TYPE_ max_hex = signedintmaxof(_TYPE_) / (0x10 / 2);
    static const _TYPE_ max_dec = signedintmaxof(_TYPE_) / (10 / 2);
    static const _TYPE_ max_oct = signedintmaxof(_TYPE_) / (010 / 2);
    static const _TYPE_ min_mag = signedintminof(_TYPE_);
    static const bool is_signed = issignedint(_TYPE_);
    const char* const start = string;
    State state = STATE_LEADING;
    bool negative = false;
    bool valid = true;
    _TYPE_ result = 0;
    _TYPE_ prior;
    char ch;

    // Iterate until the state machine returns.

    while (true) {

        //  If we have consumed all size characters, adjust the state.

        if (size <= static_cast<size_t>(string - start)) {
            switch (state) {
            case STATE_LEADING:
            case STATE_SIGN:
            case STATE_ERROR:
            case STATE_X:
                state = STATE_ERROR;
                break;
            default:
                state = STATE_END;
                break; 
            }
        }

        //  Make a state transition based on current state and next character.

        switch (state) {

        case STATE_LEADING:
            ch = *(string++);
            switch (ch) {
            case ' ':
            case '\t':
                break;
            case '+':
                state = STATE_SIGN;
                break;
            case '-':
                negative = true;
                state = STATE_SIGN;
                break;
            case '0':
                state = STATE_NONDECIMAL;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                result = ch - '0';
                state = STATE_DECIMAL;
                break;
            case '\0':
            default:
                --string;
                state = STATE_ERROR;
                break;
            }
            break;

        case STATE_SIGN:
            ch = *(string++);
            switch (ch) {
            case '0':
                state = STATE_NONDECIMAL;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                result = ch - '0';
                state = STATE_DECIMAL;
                break;
            case '\0':
            default:
                --string;
                state = STATE_ERROR;
                break;
            }
            break;

        case STATE_NONDECIMAL:
            ch = *(string++);
            switch (ch) {
            case '\0':
                state = STATE_END;
                break;
            case ' ':
            case '\t':
                state = STATE_TRAILING;
                break;
            case 'x':
            case 'X':
                state = STATE_X;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                result = ch - '0';
                state = STATE_OCTAL;
                break;
            default:
                --string;
                state = STATE_ERROR;
                break;
            }
            break;

        case STATE_DECIMAL:
            ch = *(string++);
            switch (ch) {
            case '\0':
                state = STATE_END;
                break;
            case ' ':
            case '\t':
                state = STATE_TRAILING;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (max_dec < result) {
                    valid = false;
                }
                prior = result;
                result = (result * 10) + (ch - '0');
                if (is_signed) {
                    if (prior > 0) {
                        if (0 != (result & min_mag)) {
                            if ((min_mag != result) || (!negative)) {
                                valid = false;
                            }
                        }
                    }
                }
                break;
            default:
                --string;
                state = STATE_ERROR;
                break;
            }
            break;

        case STATE_OCTAL:
            ch = *(string++);
            switch (ch) {
            case '\0':
                state = STATE_END;
                break;
            case ' ':
            case '\t':
                state = STATE_TRAILING;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                if (max_oct < result) {
                    valid = false;
                }
                result = (result * 8) + (ch - '0');
                break;
            default:
                --string;
                state = STATE_ERROR;
                break;
            }
            break;

        case STATE_X:
            ch = *(string++);
            switch (ch) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                result = ch - '0';
                state = STATE_HEXADECIMAL;
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                result = 10 + (ch - 'a');
                state = STATE_HEXADECIMAL;
                break;
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                result = 10 + (ch - 'A');
                state = STATE_HEXADECIMAL;
                break;
            case '\0':
            default:
                --string;
                state = STATE_ERROR;
                break;
            }
            break;

        case STATE_HEXADECIMAL:
            ch = *(string++);
            switch (ch) {
            case '\0':
                state = STATE_END;
                break;
            case ' ':
            case '\t':
                state = STATE_TRAILING;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (max_hex < result) {
                    valid = false;
                }
                result = (result * 16) + (ch - '0');
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                if (max_hex < result) {
                    valid = false;
                }
                result = (result * 16) + (10 + ch - 'a');
                break;
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                if (max_hex < result) {
                    valid = false;
                }
                result = (result * 16) + (10 + ch - 'A');
                break;
            default:
                --string;
                state = STATE_ERROR;
                break;
            }
            break;

        case STATE_TRAILING:
            ch = *(string++);
            switch (ch) {
            case '\0':
                state = STATE_END;
                break;
            case ' ':
            case '\t':
                break;
            default:
                --string;
                state = STATE_END;
                break;
            }
            break;

        case STATE_ERROR:
        default:
            valid = false;
            //  Fall through.
        case STATE_END:
            resultp = negative ? -result : result;
            lengthp = string - start;
            return valid;
            break;

        }
    }
}


//
//  Show this object on the output object.
//
template <typename _TYPE_>
void Number<_TYPE_>::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s widthof=%u\n", sp, widthof(_TYPE_));
    printf("%s issignedint=%d\n", sp, issignedint(_TYPE_));
    printf("%s widthof*=%u\n", sp, widthof(_TYPE_*));
}

#include "com/diag/desperado/End.h"
    
    
#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Number unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestNumber(void);
#endif


#endif
