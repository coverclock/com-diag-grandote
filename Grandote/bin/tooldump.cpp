/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the dump command line tool.
 *
 *  @see    dump
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdlib.h"
#include <unistd.h>
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/exceptions.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/desperado/Desperado.h"

#define USAGE \
    "[ -b(ytes) | -w(ords) ]" \
    " [ -o(utout) [ outname | - ] ]" \
    " [ [ inname | - ] ... ]"

int main(int argc, char **argv, char **) {
    extern char *optarg;
    extern int optind;
    int opt;
    int inerror;
    int usage;

    Print errorf(Platform::instance().error());

    char* cmdname = std::strrchr(argv[0],'/');
    if (0 != cmdname) {
        ++cmdname;
    } else {
        cmdname = argv[0];
    }

    bool help = false;
    bool words = true;
    bool debug = false;
    const char* outname = "-";

    usage = 0;
    while (0 <= (opt = ::getopt(argc, argv, "?bdo:w"))) {
        inerror = 0;
        switch (opt) {
        case '?':
            help = true;
            break;
        case 'b':
            words = false;
            break;
        case 'd':
            debug = true;
            break;
        case 'o':
            outname = optarg;
            break;
        case 'w':
            words = true;
            break;
        default:
            ++usage;
        }
        if (0 < inerror) {
            errorf("%s: bad value -- -%c %s\n",
                cmdname, opt, optarg);
            ++usage;
        }
    }

    if (help || (0 < usage)) {
        errorf("usage: %s %s\n", cmdname, USAGE);
        std::exit(help ? 0 : 1);
    }

    PathOutput* outputp;
    try {
        outputp = new PathOutput(outname);
    } catch (...) {
        outputp = 0;
    }
    if ((0 != outputp) && (0 == outputp->getFile())) {
        delete outputp;
        outputp = 0;
    }
    if (0 == outputp) {
        std::perror(outname);
        std::exit(2);
    }

    if (debug) {
        outputp->show(0, &Platform::instance().error());
    }

    Dump dump(*outputp);
    Print printf(*outputp);

    uint64_t total = 0;

    do {

        const char* inname = "-";
        if (0 != argv[optind]) {
            inname = argv[optind++];
        }

        PathInput* inputp;
        try {
            inputp = new PathInput(inname);
        } catch (...) {
            inputp = 0;
        }
        if ((0 != inputp) && (0 == inputp->getFile())) {
            delete inputp;
            inputp = 0;
        }
        if (0 == inputp) {
            std::perror(inname);
            std::exit(3);
        }

        if (debug) {
            inputp->show(0, &Platform::instance().error());
        }

        if (words) {
            total += dump.words(*inputp);
        } else {
            total += dump.bytes(*inputp);
        }

        delete inputp;

    } while (0 != argv[optind]);

    printf("bytes=%llu\n", total);

    delete outputp;

    std::exit(0);
}
