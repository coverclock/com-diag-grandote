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
 *  Implements the Word class.
 *
 *  @see    Word
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Word.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
Word::Word() :
    TransliteratorType<16>()
{
    
    // No character placed in output buffer yet.

    this->enter(    S_NONE,         '\0',       A_DONE      );
    this->enter(    S_NONE,         '\n',       A_DONENEXT  );
    this->enter(    S_NONE,         '\r',       A_DONENEXT  );
    this->enter(    S_NONE,         '!',        A_STRING    );
    this->enter(    S_NONE,         '"',        A_DOUBLE    );
    this->enter(    S_NONE,         '#',        A_COMMENT   );
    this->enter(    S_NONE,     '$',    '&',    A_STRING    );
    this->enter(    S_NONE,         '\'',       A_SINGLE    );
    this->enter(    S_NONE,     '(',    '/',    A_STRING    );
    this->enter(    S_NONE,     '0',    '9',    A_STRING    );
    this->enter(    S_NONE,     ':',    '@',    A_STRING    );
    this->enter(    S_NONE,     'A',    'Z',    A_STRING    );
    this->enter(    S_NONE,         '[',        A_STRING    );
    this->enter(    S_NONE,         '\\',       A_ESCAPE    );
    this->enter(    S_NONE,     ']',    '`',    A_STRING    );
    this->enter(    S_NONE,     'a',    'z',    A_STRING    );
    this->enter(    S_NONE,     '{',    '~',    A_STRING    );

    // Inside unquoted string.

    this->enter(    S_SOME,         '\0',       A_DONETHIS  );
    this->enter(    S_SOME,         '\t',       A_TRAILING  );
    this->enter(    S_SOME,         '\n',       A_DONETHIS  );
    this->enter(    S_SOME,         '\r',       A_DONETHIS  );
    this->enter(    S_SOME,         ' ',        A_TRAILING  );
    this->enter(    S_SOME,         '!',        A_STRING    );
    this->enter(    S_SOME,         '"',        A_DOUBLE    );
    this->enter(    S_SOME,         '#',        A_COMMENT   );
    this->enter(    S_SOME,     '$',    '&',    A_STRING    );
    this->enter(    S_SOME,         '\'',       A_SINGLE    );
    this->enter(    S_SOME,     '(',    '/',    A_STRING    );
    this->enter(    S_SOME,     '0',    '9',    A_STRING    );
    this->enter(    S_SOME,     ':',    '@',    A_STRING    );
    this->enter(    S_SOME,     'A',    'Z',    A_STRING    );
    this->enter(    S_SOME,         '[',        A_STRING    );
    this->enter(    S_SOME,         '\\',       A_ESCAPE    );
    this->enter(    S_SOME,     ']',    '`',    A_STRING    );
    this->enter(    S_SOME,     'a',    'z',    A_STRING    );
    this->enter(    S_SOME,     '{',    '~',    A_STRING    );

    // Inside single quoted string.

    this->enter(    S_SINGLE,       '\0',       A_DONETHIS  );
    this->enter(    S_SINGLE,       '\t',       A_QUOTE     );
    this->enter(    S_SINGLE,       '\n',       A_SPACE     );
    this->enter(    S_SINGLE,       '\r',       A_SPACE     );
    this->enter(    S_SINGLE,       ' ',        A_QUOTE     );
    this->enter(    S_SINGLE,       '!',        A_QUOTE     );
    this->enter(    S_SINGLE,       '"',        A_QUOTE     );
    this->enter(    S_SINGLE,       '#',        A_QUOTE     );
    this->enter(    S_SINGLE,   '$',    '&',    A_QUOTE     );
    this->enter(    S_SINGLE,       '\'',       A_UNQUOTE   );
    this->enter(    S_SINGLE,   '(',    '/',    A_QUOTE     );
    this->enter(    S_SINGLE,   '0',    '9',    A_QUOTE     );
    this->enter(    S_SINGLE,   ':',    '@',    A_QUOTE     );
    this->enter(    S_SINGLE,   'A',    'Z',    A_QUOTE     );
    this->enter(    S_SINGLE,       '[',        A_QUOTE     );
    this->enter(    S_SINGLE,       '\\',       A_ESCAPE    );
    this->enter(    S_SINGLE,   ']',    '`',    A_QUOTE     );
    this->enter(    S_SINGLE,   'a',    'z',    A_QUOTE     );
    this->enter(    S_SINGLE,   '{',    '~',    A_QUOTE     );

    // Inside double quoted string.

    this->enter(    S_DOUBLE,       '\0',       A_DONETHIS  );
    this->enter(    S_DOUBLE,       '\t',       A_QUOTE     );
    this->enter(    S_DOUBLE,       '\n',       A_SPACE     );
    this->enter(    S_DOUBLE,       '\r',       A_SPACE     );
    this->enter(    S_DOUBLE,       ' ',        A_QUOTE     );
    this->enter(    S_DOUBLE,       '!',        A_QUOTE     );
    this->enter(    S_DOUBLE,       '"',        A_UNQUOTE   );
    this->enter(    S_DOUBLE,       '#',        A_QUOTE     );
    this->enter(    S_DOUBLE,   '$',    '&',    A_QUOTE     );
    this->enter(    S_DOUBLE,       '\'',       A_QUOTE     );
    this->enter(    S_DOUBLE,   '(',    '/',    A_QUOTE     );
    this->enter(    S_DOUBLE,   '0',    '9',    A_QUOTE     );
    this->enter(    S_DOUBLE,   ':',    '@',    A_QUOTE     );
    this->enter(    S_DOUBLE,   'A',    'Z',    A_QUOTE     );
    this->enter(    S_DOUBLE,       '[',        A_QUOTE     );
    this->enter(    S_DOUBLE,       '\\',       A_ESCAPE    );
    this->enter(    S_DOUBLE,   ']',    '`',    A_QUOTE     );
    this->enter(    S_DOUBLE,   'a',    'z',    A_QUOTE     );
    this->enter(    S_DOUBLE,   '{',    '~',    A_QUOTE     );

    // Escape next character.

    this->enter(    S_ESCAPE,       '\0',       A_DONETHIS  );
    this->enter(    S_ESCAPE,   '\001', '\006', A_LITERAL   );
    this->enter(    S_ESCAPE,       '\a',       A_LITERAL   );
    this->enter(    S_ESCAPE,       '\b',       A_LITERAL   );
    this->enter(    S_ESCAPE,       '\t',       A_LITERAL   );
    this->enter(    S_ESCAPE,       '\n',       A_LITERAL   );
    this->enter(    S_ESCAPE,   '\013', '\014', A_LITERAL   );
    this->enter(    S_ESCAPE,       '\r',       A_LITERAL   );
    this->enter(    S_ESCAPE,   '\016', '\037', A_LITERAL   );
    this->enter(    S_ESCAPE,       ' ',        A_LITERAL   );
    this->enter(    S_ESCAPE,       '!',        A_ESCAPED   );
    this->enter(    S_ESCAPE,   '"',    '#',    A_LITERAL   );
    this->enter(    S_ESCAPE,   '$',    '&',    A_ESCAPED   );
    this->enter(    S_ESCAPE,       '\'',       A_LITERAL   );
    this->enter(    S_ESCAPE,   '(',    '~',    A_ESCAPED   );
    this->enter(    S_ESCAPE,       '\177',     A_LITERAL   );

    // Inside comment body.

    this->enter(    S_COMMENT,      '\0',       A_THIS      );
    this->enter(    S_COMMENT,      '\n',       A_THIS      );
    this->enter(    S_COMMENT,      '\r',       A_THIS      );
    this->enter(    S_COMMENT,      '\\',       A_IGNORE    );

    // Ignore next character.

    this->enter(    S_IGNORE,       '\0',       A_DONETHIS  );
    this->enter(    S_IGNORE,       '\t',       A_NEXT      );
    this->enter(    S_IGNORE,       '\n',       A_NEXT      );
    this->enter(    S_IGNORE,       '\r',       A_NEXT      );
    this->enter(    S_IGNORE,   ' ',    '~',    A_NEXT      );

    // Skip trailing whitespace

    this->enter(    S_WHITE,        '\0',       A_DONETHIS  );
    this->enter(    S_WHITE,        '\n',       A_DONETHIS  );
    this->enter(    S_WHITE,        '\r',       A_DONETHIS  );
    this->enter(    S_WHITE,    '!',    '~',    A_DONETHIS  );
}


//
//  Destructor.
//
Word::~Word() {
}


//
//  Show this object on the output object.
//
void Word::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->TransliteratorType<16>::show(level, display, indent + 1);
}


#include "com/diag/desperado/End.h"
