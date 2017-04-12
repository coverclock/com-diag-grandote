/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Desperadito library.

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
 *  Implements the coreable command line tool.
 *
 *  @see    coreable
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */

#include "com/diag/desperado/stdio.h"
#include <cerrno>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

int main(int argc, char **argv, char **) {
    int rc = -1;
    if (argc >= 2) {
        struct rlimit limit;
        if (-1 == getrlimit(RLIMIT_CORE, &limit)) {
            perror(argv[0]);
        } else {
            limit.rlim_cur = limit.rlim_max;
            if (-1 == setrlimit(RLIMIT_CORE, &limit)) {
                perror(argv[0]);
            }
        }
        rc = execvp(argv[1], &(argv[1]));
    } 
    return rc;
}
