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
 *  Implements the DescriptorInput class.
 *
 *  @see    DescriptorInput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/ready.h"
#include "com/diag/desperado/DescriptorInput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
DescriptorInput::DescriptorInput(int fd) :
    Input(),
    descriptor(fd),
    active(fd),
    saved(EOF),
    error(0)
{
}


//
//  Destructor.
//
DescriptorInput::~DescriptorInput() {
}


//
//  Return the next character from the file descriptor.
//
int DescriptorInput::operator() () {
    int rc = EOF;
    if (0 > this->active) {
        errno = 0;
    } else if (EOF != this->saved) {
        rc = this->saved;
        rc = intmaxof(uint8_t) & rc;
        this->saved = EOF;
    } else {
        unsigned char ch;
        ssize_t fc = ::read(this->active, &ch, sizeof(ch));
        if (0 < fc) {
            rc = ch;
            rc = rc & unsignedintmaxof(char);
        } else if (0 == fc) {
            this->active = -1;
            errno = 0;
        } else if (0 == errno) {
        	error = errno = EIO;
        } else {
        	error = errno;
        }
    }
    return rc;
}


//
//  Push a character back into the descriptor.
//
int DescriptorInput::operator() (int ch) {
    if (0 > this->active) {
        ch = EOF;
        errno = 0;
    } else {
        this->saved = ch;
    }
    return ch;
}


//
//  Read a line from the file descriptor into a buffer.
//
ssize_t DescriptorInput::operator() (char* buffer, size_t size) {
    ssize_t rc = EOF;
    if (0 > this->active) {
        errno = 0;
    } else if (0 == size) {
        rc = 0;
    } else if (1 == size) {
    	*buffer = '\0';
    	rc = 1;
    } else {
        char* here = buffer;
        ssize_t fc;
        rc = 0;
        while (1 < size) {
        	if (EOF == this->saved) {
        		fc = ::read(this->active, here, sizeof(*here));
        	} else {
                *here = this->saved;
                this->saved = EOF;
                fc = 1;
        	}
            if (0 < fc) {
                size -= fc;
                rc += fc;
                here += fc;
                if ('\n' == *(here - 1)) {
                    break;
                }
            } else if (0 == fc) {
                this->active = -1;
                if (0 == rc) {
                    rc = EOF;
                    errno = 0;
                }
                break;
            } else {
                if (0 == rc) {
                    rc = EOF;
                    if (0 == errno) {
                    	error = errno = EIO;
                    } else {
                    	error = errno;
                    }
                }
                break;
            }
        }
        if (EOF != rc) {
            *here = '\0';
            ++rc;
        }
    }
    return rc;
}


//
//  Read binary data from the file descriptor into a buffer.
//
ssize_t DescriptorInput::operator() (
    void* buffer,
    size_t minimum,
    size_t maximum
) {
    ssize_t rc = EOF;
    if (0 > this->active) {
        errno = 0;
    } else if (0 == maximum) {
        rc = 0;
    } else if ((0 == minimum) && (0 == (::desperado_descriptor_ready(this->active) & DESPERADO_DESCRIPTOR_READY_READ))) {
    	rc = 0;
    } else {
        ssize_t fc;
        rc = 0;
        size_t effective;
        while (true) {
        	if (rc >= static_cast<ssize_t>(maximum)) {
        		break; // We already have the maximum.
        	} else if (rc == 0) {
        		effective = maximum; // Haven't done a read yet but minimum is non-zero or will not block.
        	} else if (rc < static_cast<ssize_t>(minimum)) {
        		effective = maximum - rc; // Not yet minimum but try for maximum anyway.
        	} else {
        		break; // At least the minimum but would block at next read.
        	}
        	if (EOF == this->saved) {
                fc = ::read(this->active, static_cast<char*>(buffer) + rc, effective);
        	} else {
                *(static_cast<char*>(buffer) + rc) = this->saved;
                this->saved = EOF;
                fc = 1;
        	}
            if (0 < fc) {
                rc += fc;
            } else if (0 == fc) {
                this->active = -1;
                if (0 == rc) {
                    rc = EOF;
                    errno = 0;
                }
                break;
            } else {
                if (0 == rc) {
                    rc = EOF;
                    if (0 == errno) {
                    	error = errno = EIO;
                    } else {
                    	error = errno;
                    }
                }
                break;
            }
        }
    }
    return rc;
}


//
//  Show this object on the output object.
//
void DescriptorInput::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Input::show(level, display, indent + 1);
    printf("%s descriptor=%d%s\n",
        sp, this->descriptor,
        (STDIN_FILENO == this->descriptor) ? "=STDIN_FILENO" :
        (STDOUT_FILENO == this->descriptor) ? "=STDOUT_FILENO" :
        (STDERR_FILENO == this->descriptor) ? "=STDERR_FILENO" :
        "");
    printf("%s active=%d\n", sp, this->active);
    printf("%s saved=0x%08x%s\n",
        sp, this->saved,
        (EOF == this->saved) ? "=EOF" : "");
    if (0 <= this->active) {
    	int ready = ::desperado_descriptor_ready(this->active);
        printf("%s ready=0x%02x%s%s%s%s\n",
        	sp, ready,
        	(0 != (ready & DESPERADO_DESCRIPTOR_READY_READ)) ? " READ": "",
        	(0 != (ready & DESPERADO_DESCRIPTOR_READY_WRITE)) ? " WRITE": "",
        	(0 != (ready & DESPERADO_DESCRIPTOR_READY_EXCEPTION)) ? " EXCEPTION": "",
        	(0 != (ready & DESPERADO_DESCRIPTOR_READY_ERROR)) ? " ERROR": ""
        );
    }
    if (0 < this->error) {
        printf("%s error=%d=\"%s\"\n", sp, this->error, ::strerror(this->error));
     }
}


#include "com/diag/desperado/End.h"
