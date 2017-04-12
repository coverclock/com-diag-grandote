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
 *  Implements the Attribute unit test.
 *
 *  @see    Attribute
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
#include "com/diag/desperado/Attribute.h"
#include "com/diag/desperado/Attribute.h"
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
    "a\\#b=0\\#1 "
    "c\\ d\\ =2\\ 3\\   "
    "ef\t=45\t"
    "gh\t\t=67\t\t"
    "ij \t=89 \t"
    "kl\t=01\t "
    "mn\n=23\n"
    "op\n\n=45\n\n"
    "qr\t\n=67\t\n"
    "st\n\t=89\n\t"
    "uv\n=01\n "
    "wx \n=23 \n"
    "y\\z=4\\5 # End of Sequence 1\n"
    "# Beginning of Sequence 2\n"
    "\"a # b\"=\"6 # 7\" "
    "'c # d'='8 # 9'  "
    "\"e\\\"f\"\t=\"0\\\"1\"\t"
    "'g\\\'h'\t\t='2\\\'3'\t\t"
    "\"i'j\" \t=\"4'5\" \t"
    "'k\"l'\t='6\"7'\t "
    "m\\\nn=8\\\n9 "
    "o\\\np\n=0\\\n1\n"
    "qr\\\n\t=23\\\n\t"
    "\'s\n\\\rt\'=\'4\n\\\r5\' "
    "\\\nuv\n=\\\n67\n"
    "\\\nwx\t=\\\n89\t"
    "\\\ny\\\az=\\\n0\\\a1 # End of Sequence 2\n"
    "# Beginning of Sequence 3\n"
    "a\\#b=2\\#3 "
    "c\\ d\\ =4\\ 5\\   "
    "ef\t=67\t"
    "gh\t\t=89\t\t"
    "ij \t=01 \t"
    "kl\t=23\t "
    "mn\r=45\r"
    "op\r\r=67\r\r"
    "qr\t\r=89\t\r"
    "st\r\t=01\r\t"
    "uv\r=23\r "
    "wx \r=45 \r"
    "y\\z=6\\7 # End of Sequence 3\n"
    "# Beginning of Sequence 4\n"
    "\"a # b\"=\"8 # 9\" "
    "'c # d'='0 # 1'  "
    "\"e\\\"f\"\t=\"2\\\"3\"\t"
    "'g\\\'h'\t\t='4\\\'5'\t\t"
    "\"i'j\" \t=\"6'7\" \t"
    "'k\"l'\t='8\"9'\t "
    "m\\\rn=0\\\r1 "
    "o\\\rp\r=2\\\r3\r"
    "qr\\\r\t=45\\\r\t"
    "\'s\r\\\nt\'=\'6\r\\\n7\' "
    "\\\ruv\r=\\\r89\r"
    "\\\rwx\t=\\\r01\t"
    "\\\ry\\\az=\\\r2\\\a3# End of Sequence 4\n"
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
    "a#b",      "=",    "0#1",
    "c d ",     "=",    "2 3 ",
    "ef",       "=",    "45",
    "gh",       "=",    "67",
    "ij",       "=",    "89",
    "kl",       "=",    "01",
    "mn",       "=",    "23",
    "op",       "=",    "45",
    "qr",       "=",    "67",
    "st",       "=",    "89",
    "uv",       "=",    "01",
    "wx",       "=",    "23",
    "y\\z",     "=",    "4\\5",
    "a # b",    "=",    "6 # 7",
    "c # d",    "=",    "8 # 9",
    "e\"f",     "=",    "0\"1",
    "g'h",      "=",    "2'3",
    "i'j",      "=",    "4'5",
    "k\"l",     "=",    "6\"7",
    "m\nn",     "=",    "8\n9",
    "o\np",     "=",    "0\n1",
    "qr\n",     "=",    "23\n",
    "s \rt",    "=",    "4 \r5",
    "\nuv",     "=",    "\n67",
    "\nwx",     "=",    "\n89",
    "\ny\az",   "=",    "\n0\a1",
    "a#b",      "=",    "2#3",
    "c d ",     "=",    "4 5 ",
    "ef",       "=",    "67",
    "gh",       "=",    "89",
    "ij",       "=",    "01",
    "kl",       "=",    "23",
    "mn",       "=",    "45",
    "op",       "=",    "67",
    "qr",       "=",    "89",
    "st",       "=",    "01",
    "uv",       "=",    "23",
    "wx",       "=",    "45",
    "y\\z",     "=",    "6\\7",
    "a # b",    "=",    "8 # 9",
    "c # d",    "=",    "0 # 1",
    "e\"f",     "=",    "2\"3",
    "g'h",      "=",    "4'5",
    "i'j",      "=",    "6'7",
    "k\"l",     "=",    "8\"9",
    "m\rn",     "=",    "0\r1",
    "o\rp",     "=",    "2\r3",
    "qr\r",     "=",    "45\r",
    "s \nt",    "=",    "6 \n7",
    "\ruv",     "=",    "\r89",
    "\rwx",     "=",    "\r01",
    "\ry\az",   "=",    "\r2\a3",
    "",
    "",
                "=",
    "ab",       "=",
                "=",    "01",
    "cd",       "=",    "23",
    "=e=f=",    "=",    "=4=5=",
    "==g==h==", "=",    "==6==7==",
    0
};

static const char text[] = {
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "1234567890"
    "`-[];,./~!@$%^&*()_+{}|:<>?"
};

static Attribute staticAttribute;

CXXCAPI int unittestAttribute(void*, int interactive) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    Attribute::Result rc;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticAttribute.show();

    Attribute parser1;
    parser1.show();

    printf("%s[%d]: delimiter\n", __FILE__, __LINE__);

    printf("\"%s\"\n", parser1.delimiter());

    printf("%s[%d]: special\n", __FILE__, __LINE__);

    printf("\"%s\"\n", parser1.special());

    printf("%s[%d]: string\n", __FILE__, __LINE__);

    char data[sizeof(sample)];
    std::strncpy(data, sample, sizeof(data));

    int index = 0;
    char* pointer = data;
    char buffer[256];
    pointer = parser1.next(pointer, buffer, sizeof(buffer));
    while (0 != pointer) {
        if (0 == answer[index]) {
            errorf("%s[%d]: (%p!=%p[%d])!\n",
                __FILE__, __LINE__, pointer, answer[index], index);
            Platform::instance().dump().bytes(pointer, std::strlen(pointer) + 1);
            ++errors;
            break;
        } else if (0 != std::strcmp(answer[index], buffer)) {
            errorf("%s[%d]: (@%p!=@%p[%d])!\n",
                __FILE__, __LINE__, buffer, answer[index], index);
            Platform::instance().dump().bytes(buffer, std::strlen(buffer) + 1);
            Platform::instance().dump().bytes(answer[index],
                std::strlen(answer[index]) + 1);
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

    printf("%s[%d]: functor\n", __FILE__, __LINE__);

    Input* input = &Platform::instance().input();
    char testbuffer[sizeof(sample)];
    DataInput datainput;

    if (!interactive) {
        strncpy(testbuffer, sample, sizeof(testbuffer));
        DataInput newdatainput(testbuffer);
        datainput = newdatainput;
        input = &datainput;
    }

    Attribute parser2;
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
                Platform::instance().dump().bytes(buffer, std::strlen(buffer) + 1);
                Platform::instance().dump().bytes(answer[index],
                    std::strlen(answer[index]) + 1);
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
            errorf("%s[%d]: unknown %p!\n",
                __FILE__, __LINE__, rc);
            ++errors;
            break;
        }
    }

    printf("%s[%d]: lengths\n", __FILE__, __LINE__);

    char textdata[sizeof(text)];
    char textword[sizeof(text)];
    Attribute parser3;
    size_t length;

    for (size_t limit = 0; sizeof(text) >= limit; ++limit) {
        std::strncpy(textdata, text, sizeof(textdata));
        textword[0] = '?';
        pointer = parser1.next(textdata, textword, limit);
        pointer = parser1.next(pointer, textword, limit);
        if (0 != pointer) {
            errorf("%s[%d]: (%p!=%p[%u])!\n",
                __FILE__, __LINE__, 0, pointer, limit);
            Platform::instance().dump().bytes(textdata, sizeof(textdata));
            Platform::instance().dump().bytes(textword, sizeof(textword));
            ++errors;
        }
        if (0 == limit) {
            if ('?' != textword[0]) {
                errorf("%s[%d]: (%p!=%p[%u])!\n",
                    __FILE__, __LINE__, '?', textword[0], limit);
                ++errors;
            }
            continue;
        }
        if ('\0' != textword[limit - 1]) {
            errorf("%s[%d]: (%p!=%p[%u])!\n",
                __FILE__, __LINE__, '\0', textword[limit - 1], limit);
            ++errors;
            continue;
        }
        length = std::strlen(textword);
        if ((limit - 1) != length) {
            errorf("%s[%d]: (%u!=%u)!\n",
                __FILE__, __LINE__, limit - 1, length);
            Platform::instance().dump().bytes(textdata, sizeof(textdata));
            Platform::instance().dump().bytes(textword, sizeof(textword));
            ++errors;
            continue;
        }
        if (0 != std::strncmp(textdata, textword, limit - 1)) {
            errorf("%s[%d]: (@%p!=@%p[%u])!\n",
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
