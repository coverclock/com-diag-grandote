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
 *  Implements the Exception unit test.
 *
 *  @see    Exception
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Exception.h"
#include "com/diag/desperado/Exception.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Desperado.h"

#if defined(DESPERADO_HAS_EXCEPTIONS)

static void exceptional(int type) {
    std::exception e1;
    Exception e2;
    int e3 = 3;
    Exception* e4 = new DESPERADO_EXCEPTION("e4");
    Exception e5("e5", -1, __FILE__, __LINE__, DESPERADO_EXCEPTION_FUNCTION);

    switch (type) {
    case 0:
    default:
        break;
    case 1:
        throw e1;
        break;
    case 2:
        throw e2;
        break;
    case 3:
        throw e3;
        break;
    case 4:
        throw *e4;
        break;
    case 5:
        throw e5;
        break;
    }
}

static int exceptioning(int type) {
    Print printf;
    int caught = 0;

    try {
        exceptional(type);
    } catch (Exception& exception) {
        printf("%s[%d]: caught Exception \"%s\"\n",
            __FILE__, __LINE__, exception.what());
        exception.show();
        caught = 1;
    } catch (std::exception& exception) {
        printf("%s[%d]: caught std::exception \"%s\"\n",
            __FILE__, __LINE__, exception.what());
        caught = 2;
    } catch (int exception) {
        printf("%s[%d]: caught exception %d\n", __FILE__, __LINE__, exception);
        caught = exception;
    } catch (...) {
        printf("%s[%d]: caught exception\n", __FILE__, __LINE__);
        caught = 4;
    }

    return caught;
}

#endif

CXXCAPI int unittestException(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

#if defined(DESPERADO_HAS_EXCEPTIONS)

    int caught;

    printf("%s[%d]: none\n", __FILE__, __LINE__);

    caught = exceptioning(0);
    if (0 != caught) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, 0, caught);
        ++errors;
    }

    printf("%s[%d]: std::exception\n", __FILE__, __LINE__);

    caught = exceptioning(1);
    if (2 != caught) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, 2, caught);
        ++errors;
    }

    printf("%s[%d]: Exception\n", __FILE__, __LINE__);

    caught = exceptioning(2);
    if (1 != caught) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, 1, caught);
        ++errors;
    }

    printf("%s[%d]: exception\n", __FILE__, __LINE__);

    caught = exceptioning(3);
    if (3 != caught) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, 3, caught);
        ++errors;
    }

    printf("%s[%d]: DESPERADO_EXCEPTION\n", __FILE__, __LINE__);

    errno = 4;
    caught = exceptioning(4);
    if (1 != caught) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, 1, caught);
        ++errors;
    }

    printf("%s[%d]: Exception errno\n", __FILE__, __LINE__);

    errno = 5;
    caught = exceptioning(5);
    if (1 != caught) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, 1, caught);
        ++errors;
    }

    printf("%s[%d]: PathInput\n", __FILE__, __LINE__);

    const char* pathinputname = "No_such_file_or_directory.txt";

    bool pathexception = false;
    try {
        PathInput* pathinput1 = new PathInput(pathinputname, "r");
        pathinput1->show();
    } catch (Exception& e) {
        e.show();
        pathexception = true;
    }
    if (!pathexception) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__,
            true, pathexception);
        ++errors;
    }

    PathInput pathinput2;
    PathInput newpathinput2(pathinputname, "r");
    pathinput2 = newpathinput2;

    printf("%s[%d]: PathOutput\n", __FILE__, __LINE__);

    const char* pathoutputname = "/Permission_denied.txt";

    pathexception = false;
    try {
        PathOutput* pathoutput1 = new PathOutput(pathoutputname, "w");
        pathoutput1->show();
    } catch (Exception& e) {
        e.show();
        pathexception = true;
    }
    if (!pathexception) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__,
            true, pathexception);
        ++errors;
    }

    PathOutput pathoutput2;
    PathOutput newpathoutput2(pathoutputname, "w");
    pathoutput2 = newpathoutput2;

#else

    printf("%s[%d]: !defined(DESPERADO_HAS_EXCEPTIONS)\n",
        __FILE__, __LINE__);

#endif

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
