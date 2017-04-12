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
 *  Implements the FileOutput class.
 *
 *  @see    FileOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/ready.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
FileOutput::FileOutput(FILE* fp) :
    Output(),
    file(fp),
    error(0)
{
}


//
//  Destructor.
//
FileOutput::~FileOutput() {
}


//
//  Output a character.
//
int FileOutput::operator() (int c) {
    int rc = EOF;
    if (0 == this->file) {
        error= errno = EINVAL;
    } else if (std::feof(this->file)) {
        errno = 0;
    } else if (std::ferror(this->file)) {
    	if (0 == errno) {
    		error = errno = EIO;
    	} else {
    		error = errno;
    	}
    } else {
        int fc = std::fputc(c, this->file);
        if (EOF != fc) {
            rc = fc;
        } else if (std::feof(this->file)) {
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
//  Format and output a variable length argument list.
//
ssize_t FileOutput::operator() (const char* format, va_list ap) {
    ssize_t rc = EOF;
    if (0 == this->file) {
    	error= errno = EINVAL;
    } else if (std::feof(this->file)) {
        errno = 0;
    } else if (std::ferror(this->file)) {
    	if (0 == errno) {
    		error = errno = EIO;
    	} else {
    		error = errno;
    	}
    } else {
        int fc = std::vfprintf(this->file, format, ap);
        if (EOF != fc) {
            rc = fc;
        } else if (std::feof(this->file)) {
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
//  Output a string of no more than the specified size.
//
ssize_t FileOutput::operator() (const char* s, size_t size) {
    ssize_t rc = EOF;
    if (0 == this->file) {
    	error= errno = EINVAL;
    } else if (std::feof(this->file)) {
        errno = 0;
    } else if (std::ferror(this->file)) {
    	if (0 == errno) {
    		error = errno = EIO;
    	} else {
    		error = errno;
    	}
    } else if (0 == size) {
        rc = 0;
    } else {
        size_t length = ::strnlen(s, size);
        if (length < size) {
            size = length;
        }
        if (0 == size) {
            rc = 0;
        } else {
            size_t fc = std::fwrite(s, 1, size, this->file);
            if (0 != fc) {
                rc = fc;
            } else if (std::feof(this->file)) {
                errno = 0;
            } else if (0 == errno) {
            	error = errno = EIO;
            } else {
            	error = errno;
            }
        }
    }
    return rc;
}


//
//  Output binary data.
//
ssize_t FileOutput::operator() (
    const void* buffer,
    size_t minimum,
    size_t maximum
) {
    ssize_t rc = EOF;
    if (0 == this->file) {
    	error = errno = EINVAL;
    } else if (std::feof(this->file)) {
        errno = 0;
    } else if (std::ferror(this->file)) {
    	if (0 == errno) {
    		error = errno = EIO;
    	} else {
    		error = errno;
    	}
    } else if (0 == minimum) {
        rc = 0;
    } else {
    	rc = 0;
    	// We are always going to initially request the minimum, which we know
    	// is not zero. But requesting the minimum may cause the state of the
    	// standard I/O buffer to change, affecting the effective amount we can
    	// further request up to the maximum.
    	do {
			size_t fc = std::fwrite(buffer, 1, minimum, this->file);
			if (0 < fc) {
				rc += fc; // Successful.
			} else if (std::feof(this->file)) {
				rc = EOF;
				errno = 0;
				break; // End of file.
			} else if (!std::ferror(this->file)) {
				break; // Zero bytes but not End of File or Error.
			} else if (0 == errno) {
				rc = EOF;
				error = errno = EIO;
				break; // Error without an error code.
			} else {
				rc = EOF;
				error = errno;
				break; // Error.
			}
			if (fc >= maximum) {
				break; // Already have maximum.
			}
			maximum -= fc;
			size_t effective = ::desperado_file_writeable(this->file);
			if (effective < maximum) {
				maximum = effective;
			}
			if (0 == maximum) {
				break; // Nothing to request without blocking.
			}
			fc = std::fwrite(static_cast<const char*>(buffer) + fc, 1, maximum, this->file);
			if (0 < fc) {
				rc += fc; // Successful.
			} else {
				break; // Some other condition but have minimum.
			}
    	} while (false);
    }
    return rc;
}


//
//  Flush buffered data.
//
int FileOutput::operator() () {
    int rc = EOF;
    if (0 == this->file) {
    	error= errno = EINVAL;
    } else if (std::feof(this->file)) {
        errno = 0;
    } else if (std::ferror(this->file)) {
    	if (0 == errno) {
    		error = errno = EIO;
    	} else {
    		error = errno;
    	}
    } else {
        rc = std::fflush(this->file);
        if (EOF == rc) {
            if (std::feof(this->file)) {
                errno = 0;
            } else if (0 == errno) {
            	error = errno = EIO;
            } else {
            	error = errno;
            }
        }
    }
    return rc;
}


//
//  Show this object on the output object.
//
void FileOutput::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Output::show(level, display, indent + 1);
    printf("%s file=%p%s\n", sp, this->file,
        (stdout == this->file) ? "=stdout" :
        (stderr == this->file) ? "=stderr" :
        (stdin == this->file) ? "=stdin" :
        "");
    if (0 != this->file) {
        printf("%s  fileno=%d\n", sp, fileno(this->file));
        printf("%s  feof=%d\n", sp, std::feof(this->file));
        printf("%s  ferror=%d\n", sp, std::ferror(this->file));
        printf("%s  writeable=%zu\n", sp, ::desperado_file_writeable(this->file));
    }
    if (0 < this->error) {
        printf("%s error=%d=\"%s\"\n", sp, this->error, ::strerror(this->error));
     }
}


#include "com/diag/desperado/End.h"
