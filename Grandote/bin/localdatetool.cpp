/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Grandote library.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



******************************************************************************/


/**
 *  @file
 *
 *  Implements the localdate command line tool.
 *
 *  @see    localdate
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/grandote/stdlib.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/Print.h"
#include "com/diag/grandote/LocalTime.h"
#include "com/diag/grandote/TimeZone.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/Grandote.h"

int main(int, char **, char **) {
    Print printf(Platform::instance().output());
    LocalTime lt;
    lt.fromNow();
    TimeZone tz;
    const char* timezone = tz.civilian(lt.getOffset());
    char effective[4];
    std::strncpy(effective, timezone, sizeof(effective));
    if (lt.getDst() && (3 <= std::strlen(effective))) {
        effective[1] = 'D';
    }
    printf("%3.3s %3.3s %2u %02u:%02u:%02u %s %04llu\n",
        lt.getDate().weekdayToString(), lt.getDate().monthToString(),
        lt.getDay(),
        lt.getHour(), lt.getMinute(), lt.getSecond(),
        effective,
        lt.getYear());

    std::exit(0);
}
