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
 *  Implements the InputOutput unit test.
 *
 *  @see    InputOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/stdarg.h"
#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

CXXCAPI int unittestInputOutput(
    void * /* pVM */,
    char mechanism,
    Input* inputp,
    Output* outputp,
    bool verbose
) {
    Input& input = *inputp;
    Output& output = *outputp;
    Print errorf(Platform::instance().error());
    Print outputf(output);
    int errors = 0;

    errorf("%s[%d]: begin\n", __FILE__, __LINE__);

    errorf("mechanism='%c'\n", mechanism);

    errorf("%s[%d]: before\n", __FILE__, __LINE__);
    errorf(" input:\n");
    input.show(0, &(Platform::instance().error()), 2);
    errorf(" output:\n");
    output.show(0, &(Platform::instance().error()), 2);

    size_t inputted = 0;
    size_t outputted = 0;

    switch (mechanism) {

    //
    // Character.
    //
    case 'c':
        {
            int ch;
            int ch2;
            while (true) {
                ch = input();
                if (EOF == ch) {
                    if (0 != errno) {
                        errorf("%s[%d]: [%zu] input()!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input()");
                        ++errors;
                    }
                    break;
                }
                ++inputted;
                ch2 = input(ch);
                if (EOF == ch2) {
                    errorf("%s[%d]: [%zu] input(ch)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                if (ch != ch2) {
                    errorf("%s[%d]: [%zu] (0x%hhx!=0x%hhx)!\n",
                        __FILE__, __LINE__, inputted, ch, ch2);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                ch = input();
                if (EOF == ch) {
                    errorf("%s[%d]: [%zu] input()!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("input()");
                    ++errors;
                    break;
                }
                if (ch2 != ch) {
                    errorf("%s[%d]: [%zu] (0x%hhx!=0x%hhx)!\n",
                        __FILE__, __LINE__, inputted, ch2, ch);
                    std::perror("input()");
                    ++errors;
                    break;
                }
                ch2 = output(ch);
                if (EOF == ch2) {
                    errorf("%s[%d]: [%zu] output(ch)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("output(ch)");
                    ++errors;
                    break;
                }
                ++outputted;
                if (ch2 != ch) {
                    errorf("%s[%d]: [%zu] (0x%hhx!=0x%hhx)!\n",
                        __FILE__, __LINE__, inputted, ch2, ch);
                    std::perror("output(ch)");
                    ++errors;
                    break;
                }
            }
        }
        break;

    //
    // Line.
    //
    case 'l':
        {
            char buffer[257];
            ssize_t rc;
            ssize_t rc2;
            int ch;
            int ch2;
            while (true) {
                ch = input();
                if (EOF == ch) {
                    if (0 != errno) {
                        errorf("%s[%d]: [%zu] input()!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input()");
                        ++errors;
                    }
                    break;
                }
                ch2 = input(ch);
                if (EOF == ch2) {
                    errorf("%s[%d]: [%zu] input(ch)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                if (ch != ch2) {
                    errorf("%s[%d]: [%zu] (0x%hhx!=0x%hhx)!\n",
                        __FILE__, __LINE__, inputted, ch, ch2);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                rc = input(buffer, sizeof(buffer));
                if (EOF == rc) {
                    if (0 != errno) {
                        errorf(
                            "%s[%d]: [%zu] input(buffer, size)!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input(buffer, size)");
                        ++errors;
                    }
                    break;
                }
                if (0 == rc) {
                    errorf("%s[%d]: [%zu] (0==%zd)!\n",
                        __FILE__, __LINE__, inputted, rc);
                    ++errors;
                    continue;
                }
                inputted += rc;
                if (static_cast<int>(sizeof(buffer)) < rc) {
                    errorf("%s[%d]: [%zu] (%lu<%zd)!\n",
                        __FILE__, __LINE__, inputted, sizeof(buffer), rc);
                    ++errors;
                    break;
                }
                if ('\0' != buffer[rc - 1]) {
                    errorf("%s[%d]: [%zu] (NUL!=0x%hhx[%zd])!\n",
                        __FILE__, __LINE__, inputted, buffer[rc - 1], rc - 1);
                    ++errors;
                    break;
                }
                rc2 = output(buffer);
                if (EOF == rc2) {
                    errorf("%s[%d]: [%zu] output(buffer)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("output(buffer)");
                    ++errors;
                    break;
                }
                outputted += rc2;
                if ((rc - 1) != rc2) {
                    errorf("%s[%d]: [%zu] (%zd!=%zd)\n",
                        __FILE__, __LINE__, inputted, rc - 1, rc2);
                    ++errors;
                    break;
                }
            }
        }
        break;

    //
    // String.
    //
    case 's':
        {
            char buffer[16 + 1];
            ssize_t rc;
            ssize_t rc2;
            int ch;
            int ch2;
            while (true) {
                ch = input();
                if (EOF == ch) {
                    if (0 != errno) {
                        errorf("%s[%d]: [%zu] input()!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input()");
                        ++errors;
                    }
                    break;
                }
                ch2 = input(ch);
                if (EOF == ch2) {
                    errorf("%s[%d]: [%zu] input(ch)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                if (ch != ch2) {
                    errorf("%s[%d]: [%zu] (0x%hhx!=0x%hhx)!\n",
                        __FILE__, __LINE__, inputted, ch, ch2);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                rc = input(buffer, sizeof(buffer) - 1);
                if (EOF == rc) {
                    if (0 != errno) {
                        errorf(
                            "%s[%d]: [%zu] input(buffer, size)!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input(buffer, size)");
                        ++errors;
                    }
                    break;
                }
                if (0 == rc) {
                    errorf("%s[%d]: [%zu] (0==%zd)!\n",
                        __FILE__, __LINE__, inputted, rc);
                    ++errors;
                    continue;
                }
                inputted += rc;
                if (static_cast<int>(sizeof(buffer)) <= rc) {
                    errorf("%s[%d]: [%zu] (%lu<%zd)!\n",
                        __FILE__, __LINE__, inputted, sizeof(buffer), rc);
                    ++errors;
                    break;
                }
                if ('\0' != buffer[rc - 1]) {
                    errorf("%s[%d]: [%zu] (NUL!=0x%hhx[%zd])!\n",
                        __FILE__, __LINE__, inputted, buffer[rc - 1], rc - 1);
                    ++errors;
                    break;
                }
                buffer[rc - 1] = '!';
                buffer[rc] = '\0';
                rc2 = output(buffer, rc - 1);
                if (EOF == rc2) {
                    errorf(
                        "%s[%d]: [%zu] output(buffer, size)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("output(buffer, size)");
                    ++errors;
                    break;
                }
                outputted += rc2;
                if ((rc - 1) != rc2) {
                    errorf("%s[%d]: [%zu] (%zd!=%zd)\n",
                        __FILE__, __LINE__, inputted, rc - 1, rc2);
                    ++errors;
                    break;
                }
            }
        }
        break;

    //
    // Formatted.
    //
    case 'f':
        {
            char buffer[257];
            ssize_t rc;
            ssize_t rc2;
            Print outputf(output);
            int ch;
            int ch2;
            while (true) {
                ch = input();
                if (EOF == ch) {
                    if (0 != errno) {
                        errorf("%s[%d]: [%zu] input()!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input()");
                        ++errors;
                    }
                    break;
                }
                ch2 = input(ch);
                if (EOF == ch2) {
                    errorf("%s[%d]: [%zu] input(ch)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                if (ch != ch2) {
                    errorf("%s[%d]: [%zu] (0x%hhx!=0x%hhx)!\n",
                        __FILE__, __LINE__, inputted, ch, ch2);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                rc = input(buffer, sizeof(buffer));
                if (EOF == rc) {
                    if (0 != errno) {
                        errorf(
                            "%s[%d]: [%zu] input(buffer, size)!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input(buffer, size)");
                        ++errors;
                    }
                    break;
                }
                inputted += rc;
                if (0 == rc) {
                    errorf("%s[%d]: [%zu] (0==%zd)!\n",
                        __FILE__, __LINE__, inputted, rc);
                    ++errors;
                    continue;
                }
                if (static_cast<int>(sizeof(buffer)) < rc) {
                    errorf("%s[%d]: [%zu] (%lu<%zd)!\n",
                        __FILE__, __LINE__, inputted, sizeof(buffer), rc);
                    ++errors;
                    break;
                }
                if ('\0' != buffer[rc - 1]) {
                    errorf("%s[%d]: [%zu] (NUL!=0x%hhx[%zd])!\n",
                        __FILE__, __LINE__, inputted, buffer[rc - 1], rc - 1);
                    ++errors;
                    break;
                }
                rc2 = outputf("%s", buffer);
                if (EOF == rc2) {
                    errorf("%s[%d]: [%zu] outputf(buffer)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("outputf(buffer)");
                    ++errors;
                    break;
                }
                outputted += rc2;
                if ((rc - 1) != rc2) {
                    errorf("%s[%d]: [%zu] (%zd!=%zd)\n",
                        __FILE__, __LINE__, inputted, rc - 1, rc2);
                    ++errors;
                    break;
                }
            }
        }
        break;

    //
    // Binary (or maybe Block).
    //
    case 'b':
        {
            char buffer[output.minimum_buffer_size];
            ssize_t rc;
            ssize_t rc2;
            int ch;
            int ch2;
            while (true) {
                ch = input();
                if (EOF == ch) {
                    if (0 != errno) {
                        errorf("%s[%d]: [%zu] input()!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input()");
                        ++errors;
                    }
                    break;
                }
                ch2 = input(ch);
                if (EOF == ch2) {
                    errorf("%s[%d]: [%zu] input(ch)!\n",
                        __FILE__, __LINE__, inputted);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                if (ch != ch2) {
                    errorf("%s[%d]: [%zu] (0x%hhx!=0x%hhx)!\n",
                        __FILE__, __LINE__, inputted, ch, ch2);
                    std::perror("input(ch)");
                    ++errors;
                    break;
                }
                rc = input(buffer, 1, sizeof(buffer));
                if (EOF == rc) {
                    if (0 != errno) {
                        errorf(
                            "%s[%d]: [%zu] input(buffer, minimum, maximum)!\n",
                            __FILE__, __LINE__, inputted);
                        std::perror("input(buffer, minimum, maximum)");
                        ++errors;
                    }
                    break;
                }
                if (verbose) {
                	errorf(
                		"%s[%d]: input minimum=%zu maximum=%zu actual=%zd\n",
                		__FILE__, __LINE__,
                		static_cast<size_t>(1), sizeof(buffer), rc);
                }
                inputted += rc;
                if (0 == rc) {
                    errorf("%s[%d]: [%zu] (0==%zd)!\n",
                        __FILE__, __LINE__, inputted, rc);
                    ++errors;
                    continue;
                }
                if (static_cast<ssize_t>(sizeof(buffer)) < rc) {
                    errorf("%s[%d]: [%zu] (%lu<%zd)!\n",
                        __FILE__, __LINE__, inputted, sizeof(buffer), rc);
                    ++errors;
                    break;
                }
                ssize_t off = 0;
                while (off < rc) {
					rc2 = output(buffer + off, 1, rc - off);
					if (EOF == rc2) {
						errorf(
							"%s[%d]: [%zu] output(buffer, size, count)!\n",
							__FILE__, __LINE__, inputted);
						std::perror("output(buffer, size, count)");
						++errors;
						break;
					}
					if (verbose) {
						errorf(
							"%s[%d]: output minimum=%zd maximum=%zd actual=%zd\n",
							__FILE__, __LINE__, static_cast<ssize_t>(1), rc - off, rc2);
					}
					off += rc2;
					outputted += rc2;
                }
                if (outputted != inputted) {
                    errorf("%s[%d]: [%zu] (%zu!=%zu)\n",
                        __FILE__, __LINE__, inputted, outputted, inputted);
                    ++errors;
                    break;
                }
            }
        }
        break;

    default:
        errorf("%s[%d]: (0x%x)!\n",
            __FILE__, __LINE__, mechanism);
        ++errors;
        break;

    }

    errorf("%s[%d]: after\n", __FILE__, __LINE__);
    errorf(" input:\n");
    input.show(0, &(Platform::instance().error()), 2);
    errorf(" output:\n");
    output.show(0, &(Platform::instance().error()), 2);

    errorf("%s[%d]: end inputted=%zu outputted=%zu errors=%d\n",
        __FILE__, __LINE__, inputted, outputted, errors);

    return errors;
}
