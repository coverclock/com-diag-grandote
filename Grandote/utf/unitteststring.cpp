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
 *  Implements the string unit test.
 *
 *  @see    string.h
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Desperado.h"

CXXCAPI int unitteststring(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    const char string1[] = "";
    const char string2[] = "0123456789ABCDEF";
    const char string3[] = "0123456789ABCDEF";
    const char string4[] = "0123456789ABCDEF";

    size_t sl = std::strlen(string1);
    size_t snl = ::strnlen(string1, 8);
    size_t dsnl = ::desperado_strnlen(string1, 8);
    if (sl != snl) {
        errorf("%s[%d]: (%u!=%u)!\n", __FILE__, __LINE__, sl, snl);
        ++errors;
    }
    if (sl != dsnl) {
        errorf("%s[%d]: (%u!=%u)!\n", __FILE__, __LINE__, sl, dsnl);
        ++errors;
    }

    sl = 8;
    snl = ::strnlen(string2, 8);
    dsnl = ::desperado_strnlen(string2, 8);
    if (sl != snl) {
        errorf("%s[%d]: (%u!=%u)!\n", __FILE__, __LINE__, sl, snl);
        ++errors;
    }
    if (sl != dsnl) {
        errorf("%s[%d]: (%u!=%u)!\n", __FILE__, __LINE__, sl, dsnl);
        ++errors;
    }

    sl = std::strlen(string3);
    snl = ::strnlen(string3, 16);
    dsnl = ::desperado_strnlen(string3, 16);
    if (sl != snl) {
        errorf("%s[%d]: (%u!=%u)!\n", __FILE__, __LINE__, sl, snl);
        ++errors;
    }
    if (sl != dsnl) {
        errorf("%s[%d]: (%u!=%u)!\n", __FILE__, __LINE__, sl, dsnl);
        ++errors;
    }

    sl = std::strlen(string4);
    snl = ::strnlen(string4, 32);
    dsnl = ::desperado_strnlen(string4, 32);
    if (sl != snl) {
        errorf("%s[%d]: (%u!=%u)!\n", __FILE__, __LINE__, sl, snl);
        ++errors;
    }
    if (sl != dsnl) {
        errorf("%s[%d]: (%u!=%u)!\n", __FILE__, __LINE__, sl, dsnl);
        ++errors;
    }

    const char * cc = "one";
    const char aa[] = "two";
    std::string ss = "three";
    printf("%s[%d]: \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n", _s(__FILE__), __LINE__,  _s(cc), _s(aa), _s(ss), _s(&ss), _s("four"));

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
