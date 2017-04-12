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
 *  Implements the PathOutput class.
 *
 *  @see    PathOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include <limits.h>
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/exceptions.h"
#include "com/diag/desperado/Exception.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


static FILE* filepointer(const char* path, const char* mode, int* errorp) {
    FILE* fp;

    if (0 == path) {
        fp = 0;
        errno = EINVAL;
    } else if (0 == std::strncmp(path, "-", PATH_MAX)) {
        fp = stdout;
    } else {
        fp = std::fopen(path, mode);
    }
    *errorp = (0 == fp) ? errno : 0;

    return fp;
}


//
//  Constructor.
//
PathOutput::PathOutput(const char* path, const char* mode) :
    FileOutput(filepointer(path, mode, &error))
{
#if defined(DESPERADO_HAS_EXCEPTIONS)
    if ((0 != path) && (0 == this->getFile())) {
        Exception* e = new Exception(path, -(this->error), __FILE__, __LINE__,
        	__func__);
        throw *e;
    }
#endif
}


//
//  Constructor.
//
PathOutput::~PathOutput() {
    FILE* fp = this->getFile();
    if ((0 != fp) && (stdout != fp)) {
        std::fclose(fp);
    }
}


//
//  Show this object on the output object.
//
void PathOutput::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->FileOutput::show(level, display, indent + 1);
    if (0 < this->error) {
        printf("%s error=%d=\"%s\"\n", sp, this->error, ::strerror(this->error));
     }
}


#include "com/diag/desperado/End.h"
