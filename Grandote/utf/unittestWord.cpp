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
 *  Implements the Word unit test.
 *
 *  @see    Word
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Word.h"
#include "com/diag/desperado/Word.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Desperado.h"

static const char sample[]= {
    "# Beginning of Sequence 1\n"
    "a\\#b "
    "c\\ d\\   "
    "ef\t"
    "gh\t\t"
    "ij \t"
    "kl\t "
    "mn\n"
    "op\n\n"
    "qr\t\n"
    "st\n\t"
    "uv\n "
    "wx \n"
    "y\\z # End of Sequence 1\n"
    "# Beginning of Sequence 2\n"
    "\"a # b\" "
    "'c # d'  "
    "\"e\\\"f\"\t"
    "'g\\\'h'\t\t"
    "\"i'j\" \t"
    "'k\"l'\t "
    "m\\\nn "
    "o\\\np\n"
    "qr\\\n\t"
    "\'s\n\\\rt\' "
    "\\\nuv\n"
    "\\\nwx\t"
    "\\\ny\\\az # End of Sequence 2\n"
    "# Beginning of Sequence 3\n"
    "a\\#b "
    "c\\ d\\   "
    "ef\t"
    "gh\t\t"
    "ij \t"
    "kl\t "
    "mn\r"
    "op\r\r"
    "qr\t\r"
    "st\r\t"
    "uv\r "
    "wx \r"
    "y\\z # End of Sequence 3\n"
    "# Beginning of Sequence 4\n"
    "\"a # b\" "
    "'c # d'  "
    "\"e\\\"f\"\t"
    "'g\\\'h'\t\t"
    "\"i'j\" \t"
    "'k\"l'\t "
    "m\\\rn "
    "o\\\rp\r"
    "qr\\\r\t"
    "\'s\r\\\nt\' "
    "\\\ruv\r"
    "\\\rwx\t"
    "\\\ry\\\az#End of Sequence 4\n"
    "# Beginning of Sequence 5\n"
    "     \t  \r \t  \t\r \r  \r\t \r\r\t  \t \t\t \r\t\t \t\t\t\t\t\r"
    "\t\r \t\r\t\t\r\r\r  \r \t\r \r\r\t \r\t\t\r\t\r\r\r \r\r\t\r\r\r"
    "\"\" "
    "'' "
    "= "
    "ab= "
    "=01 "
    "cd=23 "
    "\\=e\\=f\\==\\=4\\=5\\= "
    "'==g==h=='=\"==6==7==\""
};

static const char* answer[] = {
    "a#b",
    "c d ",
    "ef",
    "gh",
    "ij",
    "kl",
    "mn",
    "op",
    "qr",
    "st",
    "uv",
    "wx",
    "y\\z",
    "a # b",
    "c # d",
    "e\"f",
    "g'h",
    "i'j",
    "k\"l",
    "m\nn",
    "o\np",
    "qr\n",
    "s \rt",
    "\nuv",
    "\nwx",
    "\ny\az",
    "a#b",
    "c d ",
    "ef",
    "gh",
    "ij",
    "kl",
    "mn",
    "op",
    "qr",
    "st",
    "uv",
    "wx",
    "y\\z",
    "a # b",
    "c # d",
    "e\"f",
    "g'h",
    "i'j",
    "k\"l",
    "m\rn",
    "o\rp",
    "qr\r",
    "s \nt",
    "\ruv",
    "\rwx",
    "\ry\az",
    "",
    "",
    "=",
    "ab=",
    "=01",
    "cd=23",
    "\\=e\\=f\\==\\=4\\=5\\=",
    "==g==h=====6==7==",
    0
};

static const char text[] = {
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "1234567890"
    "`-=[];,./~!@$%^&*()_+{}|:<>?"
};

static Word staticWord;

CXXCAPI int unittestWord(void*, int interactive) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    Word::Result rc;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticWord.show();

    Word parser1;
    parser1.show();

    printf("%s[%d]: delimiter\n", __FILE__, __LINE__);

    printf("\"%s\"\n", parser1.delimiter());

    printf("%s[%d]: special\n", __FILE__, __LINE__);

    printf("\"%s\"\n", parser1.special());

    printf("%s[%d]: string\n", __FILE__, __LINE__);

    char data[sizeof(sample)];
    strncpy(data, sample, sizeof(data));

    int index = 0;
    char* pointer = data;
    char buffer[256];
    pointer = parser1.next(pointer, buffer, sizeof(buffer));
    while (0 != pointer) {
        if (0 == answer[index]) {
            errorf("%s[%d]: (%p!=%p[%d])!\n",
                __FILE__, __LINE__, pointer, answer[index], index);
            Platform::instance().dump().bytes(pointer, strlen(pointer) + 1);
            ++errors;
            break;
        } else if (0 != std::strcmp(answer[index], buffer)) {
            errorf("%s[%d]: (@%p!=@%p[%d])!\n",
                __FILE__, __LINE__, buffer, answer[index], index);
            Platform::instance().dump().bytes(buffer, strlen(buffer) + 1);
            Platform::instance().dump().bytes(answer[index], strlen(answer[index]) + 1);
            ++errors;
        }
        ++index;
        pointer = parser1.next(pointer, buffer, sizeof(buffer));
    }
    if (0 != answer[index]) {
        errorf("%s[%d]: (%p!=%p[%d])!\n",
            __FILE__, __LINE__, pointer, answer[index], index);
        ++errors;
    }

    printf("%s[%d]: file\n", __FILE__, __LINE__);

    Input* input = &Platform::instance().input();
    char testbuffer[sizeof(sample)];
    DataInput datainput;

    if (!interactive) {
        strncpy(testbuffer, sample, sizeof(testbuffer));
        DataInput newdatainput(testbuffer);
        datainput = newdatainput;
        input = &datainput;
    }

    Word parser2;
    index = 0;
    while (true) {
        rc = parser2.next(*input, buffer, sizeof(buffer));
        if ((parser2.R_MORE == rc) || (parser2.R_EOF == rc)) {
            if (0 == answer[index]) {
                errorf("%s[%d]: (%p[%d])!\n",
                    __FILE__, __LINE__, answer[index], index);
                ++errors;
                break;
            } else if (0 != std::strcmp(answer[index], buffer)) {
                errorf("%s[%d]: (@%p!=@%p[%d])!\n",
                    __FILE__, __LINE__, buffer, answer[index], index);
                Platform::instance().dump().bytes(buffer, strlen(buffer) + 1);
                Platform::instance().dump().bytes(answer[index], strlen(answer[index]) + 1);
                ++errors;
            }
            ++index;
            if (parser2.R_EOF != rc) {
                continue;
            }
            if (0 != answer[index]) {
                errorf("%s[%d]: (%p[%d])!\n",
                    __FILE__, __LINE__, answer[index], index);
                ++errors;
            }
            break;
        } else if (parser2.R_EOL == rc) {
            continue;
        } else if (parser2.R_ERROR == rc) {
            errorf("%s[%d]: error!\n", __FILE__, __LINE__);
            ++errors;
            break;
        } else {
            errorf("%s[%d]: unknown 0x%x!\n",
                __FILE__, __LINE__, rc);
            ++errors;
            break;
        }
    }

    printf("%s[%d]: lengths\n", __FILE__, __LINE__);

    char textdata[sizeof(text)];
    char textword[sizeof(text)];
    Word parser3;
    size_t length;

    for (size_t limit = 0; sizeof(text) >= limit; ++limit) {
        strncpy(textdata, text, sizeof(textdata));
        textword[0] = '?';
        pointer = parser1.next(textdata, textword, limit);
        pointer = parser1.next(pointer, textword, limit);
        if (0 != pointer) {
            errorf("%s[%d]: (%p!=%p[%lu])!\n",
                __FILE__, __LINE__, 0, pointer, limit);
            Platform::instance().dump().bytes(textdata, sizeof(textdata));
            Platform::instance().dump().bytes(textword, sizeof(textword));
            ++errors;
        }
        if (0 == limit) {
            if ('?' != textword[0]) {
                errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                    __FILE__, __LINE__, '?', textword[0], limit);
                ++errors;
            }
            continue;
        }
        if ('\0' != textword[limit - 1]) {
            errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                __FILE__, __LINE__, '\0', textword[limit - 1], limit);
            ++errors;
            continue;
        }
        length = std::strlen(textword);
        if ((limit - 1) != length) {
            errorf("%s[%d]: (%lu!=%lu)!\n",
                __FILE__, __LINE__, limit - 1, length);
            Platform::instance().dump().bytes(textdata, sizeof(textdata));
            Platform::instance().dump().bytes(textword, sizeof(textword));
            ++errors;
            continue;
        }
        if (0 != std::strncmp(textdata, textword, limit - 1)) {
            errorf("%s[%d]: (@%p!=@%p[%lu])!\n",
                __FILE__, __LINE__, textdata, textword, limit);
            Platform::instance().dump().bytes(textdata, sizeof(textdata));
            Platform::instance().dump().bytes(textword, sizeof(textword));
            ++errors;
        }
    }

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__,
        errors);

    return errors;
}
