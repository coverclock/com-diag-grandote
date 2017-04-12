/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the ready command line tool that tests the functions to handle
 *  non-blocking polling I/O.
 *
 *  @see    desperado_descriptor_ready
 *  @see    desperado_file_readable
 *  @see    desperado_file_writeable
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/stdlib.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/DescriptorInput.h"
#include "com/diag/desperado/DescriptorOutput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/PathOutput.h"
#include "com/diag/desperado/ready.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/Desperado.h"


int main(int argc, char **argv, char **) {
    Print errorf(Platform::instance().error());

    char * cmdname = std::strrchr(argv[0],'/');
    cmdname = (0 != cmdname) ? cmdname + 1 : argv[0];

    bool help = false;
    bool error = false;
    bool debug = false;
    bool file = false;
    size_t inputbytes = 0;
    const char * inputfile = "-";
    size_t outputbytes = 0;
    const char * outputfile = "-";
    char * end;

    extern char *optarg;
    int opt;

    while ((opt = ::getopt(argc, argv, "?dfi:I:o:O:")) >= 0) {
        switch (opt) {
        case '?':
            help = true;
            break;
        case 'd':
            debug = true;
            break;
        case 'f':
            file = true;
            break;
        case 'i':
        	inputbytes = ::strtoul(optarg, &end, 0);
        	if (*end != '\0') {
                errorf("%s: invalid -%c %s!\n", cmdname, opt, optarg);
                error = true;
        	}
            break;
        case 'I':
        	inputfile = optarg;
            break;
        case 'o':
        	outputbytes = ::strtoul(optarg, &end, 0);
        	if (*end != '\0') {
                errorf("%s: invalid -%c %s!\n", cmdname, opt, optarg);
                error = true;
        	}
            break;
        case 'O':
        	outputfile = optarg;
            break;
        default:
            errorf("%s: invalid -%c!\n", cmdname, opt);
            error = true;
            break;
        }
    }

    if (help || error) {
        errorf("usage: %s [ -d(ebug) ] [ -f(ile) ] [ -i inputminimumbytes ] [ -I [ inputfile | - ] ] [ -o outputminimumbytes ] [ -O [ outputfile | - ] ]\n", cmdname);
        std::exit(error ? 1 : 0);
    }

    int infd = -1;
    FILE * infp = 0;
    Input * inp = 0;

    if (file) {
    	PathInput * pinp = new PathInput(inputfile, "r");
    	if (pinp->getFile() == 0) {
    		std::exit(2);
    	}
        infp = pinp->getFile();
        infd = ::fileno(infp);
        inp = pinp;
    } else {
		if (std::strcmp(inputfile, "-") == 0) {
			infd = STDIN_FILENO;
		} else if ((infd = ::open(inputfile, O_RDONLY)) < 0) {
			::perror(inputfile);
			std::exit(2);
		}
		infp = ::fdopen(infd, "r");
		inp = new DescriptorInput(infd);
    }

    Input & input = *inp;
    if (debug) {
        errorf("input: file=\"%s\" fp=%p fd=%d bytes=%zu\n", inputfile, infp, infd, inputbytes);
        input.show();
    }

    int outfd = -1;
    FILE * outfp = 0;
    Output * outp = 0;

    if (file) {
    	PathOutput * poutp = new PathOutput(outputfile, "w");
    	if (poutp->getFile() == 0) {
    		std::exit(3);
    	}
        outfp = poutp->getFile();
        outfd = ::fileno(outfp);
        outp = poutp;

    } else {
		if (std::strcmp(outputfile, "-") == 0) {
			outfd = STDOUT_FILENO;
		} else if ((outfd = ::open(outputfile, O_WRONLY | O_CREAT, 0644)) < 0) {
			::perror(argv[2]);
			std::exit(3);
		}
		outfp = ::fdopen(outfd, "w");
		outp = new DescriptorOutput(outfd);

    }

    Output & output = *outp;
    if (debug) {
    	errorf("output: file=\"%s\" fp=%p fd=%d bytes=%zu\n", outputfile, outfp, outfd, outputbytes);
    	output.show();
    }

    char buffer[256];
    ssize_t inputted;
    ssize_t outputted;
    const char * bb;

    int ready;
    size_t readable;
    size_t writeable;

    while (true) {

    	if (debug) {
			ready = desperado_descriptor_ready(infd);
			readable = desperado_file_readable(infp);
			writeable = desperado_file_writeable(infp);
			errorf("input: ready=0x%x%s%s%s%s readable=%zu writeable=%u\n",
				ready,
				((ready & DESPERADO_DESCRIPTOR_READY_READ) != 0) ? " READ": "",
				((ready & DESPERADO_DESCRIPTOR_READY_WRITE) != 0) ? " WRITE": "",
				((ready & DESPERADO_DESCRIPTOR_READY_EXCEPTION) != 0) ? " EXCEPTION": "",
				((ready & DESPERADO_DESCRIPTOR_READY_ERROR) != 0) ? " ERROR": "",
				readable, writeable
			);
    	}

    	inputted = input(buffer, (inputbytes < sizeof(buffer)) ? inputbytes : sizeof(buffer), sizeof(buffer));
    	if (debug) {
    		errorf("input: bytes=%zd\n", inputted);
    	}
    	if (inputted < 0) {
    		std::exit(4);
    	} else if (inputted == 0) {
			Platform::instance().yield(Platform::instance().frequency(), true);
			continue;
    	}

    	if (debug) {
			ready = desperado_descriptor_ready(outfd);
			readable = desperado_file_readable(outfp);
			writeable = desperado_file_writeable(outfp);
			errorf("output: ready=0x%x%s%s%s%s readable=%zu writeable=%zu\n",
				ready,
				((ready & DESPERADO_DESCRIPTOR_READY_READ) != 0) ? " READ": "",
				((ready & DESPERADO_DESCRIPTOR_READY_WRITE) != 0) ? " WRITE": "",
				((ready & DESPERADO_DESCRIPTOR_READY_EXCEPTION) != 0) ? " EXCEPTION": "",
				((ready & DESPERADO_DESCRIPTOR_READY_ERROR) != 0) ? " ERROR": "",
				readable, writeable
			);
    	}

    	bb = buffer;
    	while (inputted > 0) {
    		outputted = output(bb, (outputbytes < static_cast<size_t>(inputted)) ? outputbytes : static_cast<size_t>(inputted), static_cast<size_t>(inputted));
        	if (debug) {
        		errorf("output: bytes=%zd\n", outputted);
        	}
    		if (outputted < 0) {
    			std::exit(5);
    		} else if (outputted > 0) {
    			bb += outputted;
    			inputted -= outputted;
    		} else {
    			Platform::instance().yield(Platform::instance().frequency(), true);
    		}
    	}


    }

    std::exit(0);
}
