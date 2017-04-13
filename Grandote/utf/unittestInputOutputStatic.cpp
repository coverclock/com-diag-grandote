/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Grandote library.
    
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
 *  Implements the InputOutput Static unit test.
 *
 *  @see    Input
 *  @see    Output
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/grandote/stdarg.h"
#include "com/diag/grandote/UnitTest.h"
#include "com/diag/grandote/errno.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/BufferInput.h"
#include "com/diag/grandote/BufferInput.h"
#include "com/diag/grandote/BufferOutput.h"
#include "com/diag/grandote/BufferOutput.h"
#include "com/diag/grandote/DataInput.h"
#include "com/diag/grandote/DataInput.h"
#include "com/diag/grandote/DescriptorInput.h"
#include "com/diag/grandote/DescriptorInput.h"
#include "com/diag/grandote/DescriptorOutput.h"
#include "com/diag/grandote/DescriptorOutput.h"
#include "com/diag/grandote/DumpInput.h"
#include "com/diag/grandote/DumpInput.h"
#include "com/diag/grandote/DumpOutput.h"
#include "com/diag/grandote/DumpOutput.h"
#include "com/diag/grandote/FileInput.h"
#include "com/diag/grandote/FileInput.h"
#include "com/diag/grandote/FileOutput.h"
#include "com/diag/grandote/FileOutput.h"
#include "com/diag/grandote/Input.h"
#include "com/diag/grandote/Input.h"
#include "com/diag/grandote/Output.h"
#include "com/diag/grandote/Output.h"
#include "com/diag/grandote/PathInput.h"
#include "com/diag/grandote/PathInput.h"
#include "com/diag/grandote/PathOutput.h"
#include "com/diag/grandote/PathOutput.h"
#include "com/diag/grandote/SyslogOutput.h"
#include "com/diag/grandote/SyslogOutput.h"
#include "com/diag/grandote/Print.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/Grandote.h"

static BufferInput staticBufferInput;
static BufferOutput staticBufferOutput;
static DataInput staticDataInput;
static DescriptorInput staticDescriptorInput;
static DescriptorOutput staticDescriptorOutput;
static DumpInput staticDumpInput;
static DumpOutput staticDumpOutput;
static FileInput staticFileInput;
static FileOutput staticFileOutput;
static Input staticInput;
static Output staticOutput;
static PathInput staticPathInput;
static PathOutput staticPathOutput;
#if defined(GRANDOTE_HAS_SYSLOG)
static SyslogOutput staticSyslogOutput;
#endif

CXXCAPI int unittestInputOutputStatic(void) {
    Print printf(Platform::instance().output());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticBufferInput.show();
    ::staticBufferOutput.show();
    ::staticDataInput.show();
    ::staticDescriptorInput.show();
    ::staticDescriptorOutput.show();
    ::staticDumpInput.show();
    ::staticDumpOutput.show();
    ::staticFileInput.show();
    ::staticFileOutput.show();
    ::staticInput.show();
    ::staticOutput.show();
    ::staticPathInput.show();
    ::staticPathOutput.show();
#if defined(GRANDOTE_HAS_SYSLOG)
    ::staticSyslogOutput.show();
#endif

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
