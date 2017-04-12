#ifndef _COM_DIAG_DESPERADO_TRANSLITERATER_H_
#define _COM_DIAG_DESPERADO_TRANSLITERATER_H_

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
 *  Declares the Transliterator class.
 *
 *  @see    Transliterator
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a table driven transliterator useful in constructing
 *  pretty complex yet efficient parsers, implementing two interfaces: one
 *  for parsing from a buffer, another for parsing from an input functor.
 *
 *  The code may appear to be somewhat redundant: the semantics of input
 *  functor versus buffer parsing is subtly different, and putting some
 *  of that code into a common function would have added a function call
 *  per character. This may be harder to maintain, but faster.
 *
 *  The methods are implemented as a finite state machine with a
 *  stack, which is equivalent to a push down automata (PDA), implying
 *  that the language it parses must be LL(1) (as I dimly recall from
 *  those courses in compiler design, formal languages, and automata
 *  theory).
 *
 *  Note that this class cannot be used by itself. It must be used
 *  via inheritance or composition by an application which enters
 *  state, character, and action triplets into the transliterator
 *  table.
 *
 *  @see    Word
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Transliterator : public Object {

public:

    /**
     *  The PDA may be in any of these states.
     */
    enum State {
        S_NONE          = 0,        // No characters in output buffer
        S_FIRST         = S_NONE,   // First state
        S_INITIAL       = S_NONE,   // Initial state
        S_SOME          = 1,        // Some characters in output buffer
        S_SINGLE        = 2,        // Inside single quotation
        S_DOUBLE        = 3,        // Inside double quotation
        S_ESCAPE        = 4,        // Escaped character
        S_COMMENT       = 5,        // Inside comment
        S_IGNORE        = 6,        // Ignored character
        S_WHITE         = 7,        // Whitespace
        S_DONE          = 8,        // Done
        S_LAST          = S_DONE    // Last state
    };

    /**
     *  Constructor.
     *
     *  @param  st          points to a State stack (array) provided by the
     *                      caller.
     *
     *  @param  co          is the count of States in the state stack.
     */
    explicit Transliterator(uint8_t* st = 0, unsigned int co = 0);

    /**
     *  Destructor.
     */
    virtual ~Transliterator();

    /**
     *  An input stimulus to the PDA is of this type.
     */
    typedef unsigned char Stimulus;

    /**
     *  The PDA may take stimuli in the range first to last as shown
     *  below. The special EOF value returned by the Input functor indicates
     *  end of file.
     */
    enum {
        I_EOF           = EOF,
        I_FIRST         = 0,
        I_LAST          = (1 << (sizeof(Stimulus) * 8)) - 1
    };

    /**
     *  The PDA may take any of these actions.
     */
    enum Action {
        A_SKIP          = 0,        // Skip this character
        A_FIRST         = A_SKIP,   // First action
        A_INITIAL       = A_SKIP,   // Initial action
        A_STRING        = 1,        // Capture string of characters
        A_CHARACTER     = 2,        // Capture single character
        A_SINGLE        = 3,        // Begin single quote
        A_DOUBLE        = 4,        // Begin double quote
        A_UNQUOTE       = 5,        // End matching quote
        A_NEXT          = 6,        // Restore previous context with next char
        A_THIS          = 7,        // Restore previous contex with this char
        A_ESCAPE        = 8,        // Escape character
        A_QUOTE         = 9,        // Quote following characters
        A_LITERAL       = 10,       // Take next character literally
        A_ESCAPED       = 11,       // Escape next character
        A_COMMENT       = 12,       // Comment
        A_TRAILING      = 13,       // Skip trailing whitespace
        A_IGNORE        = 14,       // Ignore next character
        A_SPACE         = 15,       // Treat this character like a space
        A_DONETHIS      = 17,       // Done, pick up this character
        A_DONENEXT      = 18,       // Done, pick up next character
        A_DONE          = 19,       // Done, input exhausted
        A_LAST          = A_DONE    // Last action
    };

    /**
     *  Enter an action into the transliterator table at the
     *  specified state and input.
     *
     *  @param  s       is the state index.
     *  @param  c       is the input index.
     *  @param  a       is the action to enter.
     */
    void enter(State s, Stimulus c, Action a);

    /**
     *  Enter the same action into the transliterator table at
     *  the specified state and within the inclusive range of
     *  inputs.
     *
     *  @param  s       is the state index.
     *  @param  f       is the first (low) input index.
     *  @param  l       is the last (high) input index.
     *  @param  a       is the action to enter.
     */
    void enter(State s, Stimulus f, Stimulus l, Action a);

    /**
     *  This method parses out the next "word" in the user buffer.
     *  The buffer must contain a C-style null-terminated ASCII string.
     *  All parsing halts at the end of the string, and the state of
     *  parsing is not maintained across calls. It is assumed that the
     *  output buffer is large enough to hold the word. If not, the
     *  word is silently truncated. (The caller can avoid this by
     *  allocating an output buffer at least as large as the input
     *  buffer.)
     *
     *  @param  in      points to the starting location in the input buffer.
     *  @param  out     points to the starting location in the output buffer.
     *  @param  size    is size of the output buffer in bytes.
     *
     *  @return the new pointer to the input buffer, which may be used as
     *          as the in pointer on a subsequent call; or NULL if at the
     *          end of the input buffer, in which case the output buffer
     *          contains nothing useful.
     */
    virtual char* next(char* in, char* out, size_t size);

    /**
     *  A transliterator using an input functor may return one of
     *  these results.
     */
    enum Result {
        R_ERROR         = -2,       // Error
        R_EOF           = -1,       // End Of File
        R_EOL           = 0,        // End Of Line
        R_MORE          = 1         // More to come
    };

    /**
     *  This method returns the next "word" in the user input stream.
     *  A "word" may extend beyond physical lines by escaping terminating
     *  newlines. It is assumed that the output buffer is large enough
     *  to contain the complete word.
     *
     *  Note that the caller has full control over the position of the
     *  seek pointer for the input stream; the caller can, for example,
     *  skip all remaining data in the current line after retrieving the
     *  first word iff the functions returns R_MORE (otherwise the newline
     *  at the end as already been consumed by the parser).
     *
     *  @param  input   refers to an input functor.
     *  @param  out     points to the starting location in the output buffer.
     *  @param  size    is the size of the output buffer in bytes.
     *
     *  @return R_MORE if more data may exist on current line in the input
     *          stream; R_EOF if end of file; R_EOL if the end of the current
     *          line has been reached; R_ERROR if an error such as stack
     *          overflow has occurred, although the output buffer may contain
     *          a partial result, and a subsequent call may be made to
     *          continue parsing.
     */
    virtual Result next(Input& input, char* out, size_t size);

    /**
     *  Clears the error counter.
     */
    void clear();

    /**
     *  Returns true if the error counter is non-zero. The error
     *  counter is cleared every time the transliterator is called,
     *  and incremented when ever the transliterator encounters a
     *  problem such as buffer overrun, stack overflow, etc.
     *
     *  @return true if the error counter is non-zero.
     */
    bool errored() const;

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

protected:

    /**
     *  Increment the error counter.
     */
    void error();

    /**
     *  Initialize the PDA.
     *
     *  @param  s       is the initial state to push into the context
     *                  stack.
     */
    void prime(State s);

    /**
     *  Push a state into the context stack.
     *
     *  @param  s       is the state to push into the context stack.
     */
    void push(State s);

    /**
     *  Pop a state from the context stack.
     *
     *  @return         the state popped from the context stack.
     */
    State pop();

    /**
     *  Replace the top state on the context stack with another state.
     *
     *  @param  s       is the state that replaces the top state on
     *                  the context stack.
     */
    void poke(State s);

    /**
     *  Return the top state in the context stack without popping it.
     *
     *  @return         the top state on the context stack.
     */
    State peek() const;

    /**
     *  Append a character into the output buffer.
     *
     *  @param  c       is the character to append.
     *  @param  to      is the output buffer pointer, which is
     *                  incremented.
     *  @param  size    is the number of available bytes in the
     *                  output buffer, which is decremented.
     */
    void append(char c, char* &to, size_t &size);

    /**
     *  Insert a character into the output buffer, which does
     *  not advance the output buffer pointer.
     *
     *  @param  c       is the character to append.
     *  @param  to      is the output buffer pointer, which is
     *                  left unchanged.
     *  @param  size    is the number of available bytes in the
     *                  output buffer, which is left unchanged.
     */
    void insert(char c, char* to, size_t size);

private:

    /**
     *  Copy constructor.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Transliterator(const Transliterator& that);

    /**
     *  Assignment operator.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Transliterator& operator=(const Transliterator& that);

    /**
     *  This is the transliterator table.
     */
    uint8_t table[S_LAST - S_FIRST + 1][I_LAST - I_FIRST + 1];

    /**
     *  This points to the bottom (beginning or lowest memory
     *  address) of the array used as a stack of context states.
     */
    uint8_t* stack;

    /**
     *  This points to the top of the context stack.    
     */
    uint8_t* sp;

    /**
     *  This points past the highest entry in the context stack.
     */
    uint8_t* spp;

    /**
     *  This is the number of entries in the context stack.
     */
    unsigned int contexts;

    /**
     *  This is the error counter.
     */
    unsigned int errors;

};


//
//  Clear the error counter.
//
inline void Transliterator::clear() {
    this->errors = 0;
}


//
//  Increment the error counter.
//
inline void Transliterator::error() {
    ++(this->errors);
}


//
//  Return true if the error counter is non-zero.
//
inline bool Transliterator::errored() const {
    return (this->errors > 0);
}


//
//  Enter a single entry in the transliterator table.
//
inline void Transliterator::enter(State s, Stimulus c, Action a) {
    if ((S_FIRST <= s) && (s <= S_LAST)) {
        this->table[s][c] = static_cast<uint8_t>(a);
    } else {
        this->error();
    }
}


//
//  Enter many similar entries in the transliterator table.
//
inline void Transliterator::enter(State s, Stimulus f, Stimulus l, Action a) {
    for (int ch = f; ch <= l; ++ch) {
        this->enter(s, static_cast<Stimulus>(ch), a);
    }
}


//
//  Initialize the transliterator.
//
inline void Transliterator::prime(State s) {
    this->sp = this->stack;
    this->spp = this->stack + this->contexts;
    *(this->sp) = static_cast<uint8_t>(s);
    this->clear();
}


//
//  Push a state onto the context stack.
//
inline void Transliterator::push(State s) {
    if (this->sp < this->spp) {
        *(++(this->sp)) = static_cast<uint8_t>(s);
    } else {
        this->error();
    }
}


//
//  Pop a state from the context stack.
//
inline Transliterator::State Transliterator::pop() {
    State s;

    if (this->sp > this->stack) {
        s = static_cast<State>(*((this->sp)--));
    } else {
        s =  static_cast<State>(*(this->sp));
        this->error();
    }

    return s;
}


//
//  Poke a state over the top state on the context stack.
//
inline void Transliterator::poke(State s) {
    *(this->sp) = s;
}


//
//  Peek at the top state on the context stack.
//
inline Transliterator::State Transliterator::peek() const {
    return static_cast<State>(*(this->sp));
}


//
//  Append a character onto the output buffer.
//
inline void Transliterator::append(char c, char* &to, size_t &size){
    if (0 < size) {
        *(to++) = c;
        --size;
    } else {
        this->error();
    }
}


//
//  Insert a character into the output buffer.
//
inline void Transliterator::insert(char c, char* to, size_t size) {
    if (0 < size) {
        *to = c;
    } else {
        this->error();
    }
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Transliterator unit test.
 *
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestTransliterator(void);
#endif


#endif
