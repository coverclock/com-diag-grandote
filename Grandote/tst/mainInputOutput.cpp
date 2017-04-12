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
 *  Implements the InputOutput unit test main program.
 *
 *  @see    InputOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/DescriptorInput.h"
#include "com/diag/desperado/DescriptorInput.h"
#include "com/diag/desperado/DescriptorOutput.h"
#include "com/diag/desperado/DescriptorOutput.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/BufferInput.h"
#include "com/diag/desperado/BufferInput.h"
#include "com/diag/desperado/BufferOutput.h"
#include "com/diag/desperado/BufferOutput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/desperado/DumpInput.h"
#include "com/diag/desperado/DumpInput.h"
#include "com/diag/desperado/DumpOutput.h"
#include "com/diag/desperado/DumpOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/reads.h"
#include "com/diag/desperado/reads.h"
#include "com/diag/desperado/writes.h"
#include "com/diag/desperado/writes.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/Desperado.h"

#define USAGE   \
    "[ -d(ebug) ] [ -v(erbose) ]" \
    " [ -c(haracter) | -l(ine) | -b(lock) | -s(tring) | -f(ormatted) ]" \
    " [ -D(escriptor) | -F(ile) | -I(pc) | -N(ull) | -P(ath) | -S(tring) | -B(lock) ]" \
    " [ -o(utout) [ outname | - ] ]" \
    " [ [ inname | - ] ... ]"

int main(int argc, char **argv, char **) {
    extern char *optarg;
    extern int optind;
    int opt;
    int usage;
    int rc;

    int errors = 0;

    bool debug = false;
    bool verbose = false;
    bool help = false;

    const char *outname = "-";
    char method = 0;
    char object = 0;
    char* cmdname;

    Print errorf(Platform::instance().error());

    errorf("%s[%d]: begin\n", __FILE__, __LINE__);

    cmdname = (0 == (cmdname = std::strrchr(argv[0],'/')))
                ? argv[0] : cmdname + 1;

    usage = 0;
    while (0 <= (opt = ::getopt(argc, argv, "?bcdflo:rsvBDFINPS"))) {
        switch (opt) {
        case '?':
            help = true;
            break;
        case 'd':
            debug = true;
            break;
        case 'b':
        case 'c':
        case 'f':
        case 'l':
        case 's':
            method = opt;
            break;
        case 'B':
        case 'D':
        case 'F':
        case 'I':
        case 'N':
        case 'P':
        case 'S':
            object = opt;
            break;
        case 'o':
            outname = optarg;
            break;
        case 'v':
        	verbose = true;
        	break;
        default:
            ++usage;
            break;
        }
    }

    if (help || (0 < usage) || (0 == method) || (0 == object)) {
        errorf("usage: %s %s\n", cmdname, USAGE);
        std::exit(help ? 0 : 1);
    }

    Output* outputp = 0;
    FILE* outfile = 0;
    int outsocket = EOF;
    Dump dump;
    DumpOutput dumpoutput;
    DumpInput dumpinput;

    switch (object) {

    case 'B':
        if (0 == std::strcmp(outname, "-")) {
            outfile = stdout;
        } else {
            outfile = std::fopen(outname, "w");
        }
        if (0 != outfile) {
            outsocket = fileno(outfile);
        } else {
            std::perror(outname);
            ++errors;
        }
        break;

    case 'D':
        if (0 == std::strcmp(outname, "-")) {
            outsocket = 1;
        } else {
            outsocket = ::open(outname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        }
        if (0 <= outsocket) {
            outputp = new DescriptorOutput(outsocket);
        } else {
            std::perror(outname);
            ++errors;
        }
        break;

    case 'F':
        if (0 == std::strcmp(outname, "-")) {
            outfile = stdout;
        } else {
            outfile = std::fopen(outname, "w");
        }
        if (0 != outfile) {
            outsocket = fileno(outfile);
            outputp = new FileOutput(outfile);
        } else {
            std::perror(outname);
            ++errors;
        }
        break;

    case 'N':
        outputp = new Output;
        break;

    case 'P':
        try {
            outputp = new PathOutput(outname);
        } catch (...) {
            outputp = 0;
            std::perror(outname);
            ++errors;
        }
        break;

    case 'S':
        if (0 == std::strcmp(outname, "-")) {
            outfile = stdout;
        } else {
            outfile = std::fopen(outname, "w");
        }
        if (0 != outfile) {
            outsocket = fileno(outfile);
        } else {
            std::perror(outname);
            ++errors;
        }
        break;

    }

    const char *inname = "-";
    FILE* infile = 0;
    int insocket = EOF;
    char* instring = 0;
    char* outstring = 0;
    Heap heap;
    struct stat status;
    size_t size = 0;
    ssize_t fc;
    Input* inputp = 0;
    BufferInput* si = 0;
    BufferOutput* so = 0;
    DataInput* bi = 0;

    do {

        if (0 == argv[optind]) {
            inname = "-";
        } else {
            inname = argv[optind++];
        }

        switch (object) {

        case 'B':
            do {
                if (0 == std::strcmp(inname, "-")) {
                    infile = stdin;
                } else {
                    infile = std::fopen(inname, "r");
                }
                if (0 == infile) {
                    std::perror(inname);
                    ++errors;
                    break;
                }
                insocket = fileno(infile);
                if (0 != ::fstat(insocket, &status)) {
                    std::perror("fstat");
                    ++errors;
                    break;
                }
                size = status.st_size;
                instring = static_cast<char*>(heap.malloc(size));
                if (0 == instring) {
                    std::perror("instring=heap.malloc(size)");
                    ++errors;
                    break;
                }
                bi = new DataInput(instring, size);
                inputp = bi;
                if (0 == inputp) {
                    std::perror("new DataInput(instring,size)");
                    ++errors;
                    break;
                }
                fc = ::desperado_reads(insocket, const_cast<char*>(static_cast<const char*>(bi->getData())),
                		bi->getSize(), bi->getSize());
                if (static_cast<ssize_t>(size) != fc) {
                    heap.free(instring);
                    instring = 0;
                    std::perror("desperado_reads(insocket,instring,size,size)");
                    ++errors;
                    break;
                }
                outstring = static_cast<char*>(heap.malloc(size));
                if (0 == outstring) {
                    std::perror("outstring=heap.malloc(size)");
                    ++errors;
                    break;
                }
                so = new BufferOutput(outstring, size);
                outputp = so;
                if (0 == outputp) {
                    std::perror("new BufferInput(outstring,size)");
                    ++errors;
                    break;
                }
            } while (false);
            break;

        case 'D':
            if (0 == std::strcmp(inname, "-")) {
                insocket = 0;
            } else {
                insocket = ::open(inname, O_RDONLY);
            }
            if (0 <= insocket) {
                inputp = new DescriptorInput(insocket);
            } else {
                std::perror(inname);
                ++errors;
            }
            break;

        case 'F':
            if (0 == std::strcmp(inname, "-")) {
                infile = stdin;
            } else {
                infile = std::fopen(inname, "r");
            }
            if (0 != infile) {
                insocket = fileno(infile);
                inputp = new FileInput(infile);
            } else {
                std::perror(inname);
                ++errors;
            }
            break;

        case 'N':
            inputp = new Input;
            break;

        case 'P':
            try {
                inputp = new PathInput(inname);
            } catch (...) {
                inputp = 0;
                std::perror(inname);
                ++errors;
            }
            break;

        case 'S':
            do {
                if (0 == std::strcmp(inname, "-")) {
                    infile = stdin;
                } else {
                    infile = std::fopen(inname, "r");
                }
                if (0 == infile) {
                    std::perror(inname);
                    ++errors;
                    break;
                }
                insocket = fileno(infile);
                if (0 != ::fstat(insocket, &status)) {
                    std::perror("fstat");
                    ++errors;
                    break;
                }
                size = status.st_size;
                instring = static_cast<char*>(heap.malloc(size));
                if (0 == instring) {
                    std::perror("instring=heap.malloc(size)");
                    ++errors;
                    break;
                }
                si = new BufferInput(instring, size);
                inputp = si;
                if (0 == inputp) {
                    std::perror("new BufferInput(instring,size)");
                    ++errors;
                    break;
                }
                fc = ::desperado_reads(insocket, si->getBuffer(),
                        si->getSize(), si->getSize());
                if (static_cast<ssize_t>(size) != fc) {
                    heap.free(instring);
                    instring = 0;
                    std::perror("desperado_reads(insocket,instring,size,size)");
                    ++errors;
                    break;
                }
                outstring = static_cast<char*>(heap.malloc(size));
                if (0 == outstring) {
                    std::perror("outstring=heap.malloc(size)");
                    ++errors;
                    break;
                }
                so = new BufferOutput(outstring, size);
                outputp = so;
                if (0 == outputp) {
                    std::perror("new BufferInput(outstring,size)");
                    ++errors;
                    break;
                }
            } while (false);
            break;

        }

        if ((0 == inputp) || (0 == outputp)) {
            ::exit(errors);
        }

        if (debug) {
            DumpOutput newdumpoutput(*outputp, dump);
            DumpInput newdumpinput(*inputp, dump);
        	dumpoutput = newdumpoutput;
        	dumpinput = newdumpinput;
            inputp = &dumpinput;
            outputp = &dumpoutput;
        }

        rc = unittestInputOutput(0, method, inputp, outputp, verbose);
        errors += rc;

        if (debug) {
            inputp = &(dumpinput.input());
            outputp = &(dumpoutput.output());
        }

        if (0 != outstring) {
            fc = ::desperado_writes(outsocket, so->getBuffer(),
                    so->getOffset(), so->getOffset());
            if (static_cast<ssize_t>(so->getOffset()) != fc) {
                std::perror("desperado_writes(outoutsocket,outstring,size)");
                ++errors;
            }
            delete outputp;
            outputp = 0;
            heap.free(outstring);
            outstring = 0;
        }

        if (0 != inputp) {
            delete inputp;
            inputp = 0;
        }

        if (0 != instring) {
            heap.free(instring);
            instring = 0;
        }

        if (0 != infile) {
            std::fclose(infile);
            infile = 0;
            insocket = EOF;
        }

        if (EOF != insocket) {
            ::close(insocket);
            insocket = EOF;
        }

    } while (0 != argv[optind]);

    if (0 != outputp) {
        delete outputp;
        outputp = 0;
    }

    if (0 != outfile) {
        std::fclose(outfile);
        outfile = 0;
        outsocket = EOF;
    }

    if (EOF != outsocket) {
        ::close(outsocket);
        outsocket = EOF;
    }

    errorf("%s[%d]: end errors=%d\n", __FILE__, __LINE__, errors);

    ::exit(errors);
}
