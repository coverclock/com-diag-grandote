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

    $Name:  $


******************************************************************************/


/**
 *  @file
 *
 *  Implements the Vintage class.
 *
 *  @see    Vintage
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */



#include "com/diag/desperado/release.h"
#include "com/diag/desperado/Vintage.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"


const char Vintage::product[] =     "$Product: " DESPERADO_VINTAGE_PRODUCT " $";

const char Vintage::license[] =     "$License: " DESPERADO_VINTAGE_LICENSE " $";

const char Vintage::copyright[] =   "$Copyright: " DESPERADO_VINTAGE_COPYRIGHT " $";

const char Vintage::contact[] =     "$Contact: " DESPERADO_VINTAGE_CONTACT " $";

const char Vintage::url[] =         "$Url: " DESPERADO_VINTAGE_URL " $";

const char Vintage::date[] =        "$Date: " DESPERADO_VINTAGE_DATE " $";

const char Vintage::release[] =     "$Release: " DESPERADO_VINTAGE_RELEASE " $";

const char Vintage::version[] =     "$Version: " DESPERADO_VINTAGE_VERSION " $";

const char Vintage::target[] =     "$Target: " DESPERADO_VINTAGE_TARGET " $";

const char Vintage::platform[] =     "$Platform: " DESPERADO_VINTAGE_PLATFORM " $";

const char Vintage::build[] =       "$Build: " __DATE__ " " __TIME__ " $";


//
//  Destructor.
//
Vintage::~Vintage() {
}


//
//  Show this object on the output object.
//
void Vintage::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s product[%lu]=\"%s\"\n",
        sp, sizeof(this->product), this->product);
    printf("%s license[%lu]=\"%s\"\n",
        sp, sizeof(this->license), this->license);
    printf("%s copyright[%lu]=\"%s\"\n",
        sp, sizeof(this->copyright), this->copyright);
    printf("%s contact[%lu]=\"%s\"\n",
        sp, sizeof(this->contact),  this->contact);
    printf("%s url[%lu]=\"%s\"\n",
        sp, sizeof(this->url),  this->url);
    printf("%s date[%lu]=\"%s\"\n",
        sp, sizeof(this->date),  this->date);
    printf("%s release[%lu]=\"%s\"\n",
        sp, sizeof(this->release), this->release);
    printf("%s version[%lu]=\"%s\"\n",
        sp, sizeof(this->version), this->version);
    printf("%s target[%lu]=\"%s\"\n",
        sp, sizeof(this->target), this->target);
    printf("%s platform[%lu]=\"%s\"\n",
        sp, sizeof(this->platform), this->platform);
    printf("%s build[%lu]=\"%s\"\n",
        sp, sizeof(this->build), this->build);
}


#include "com/diag/desperado/End.h"
