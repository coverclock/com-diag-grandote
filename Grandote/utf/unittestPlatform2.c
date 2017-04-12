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
 *  Implements the Platform2 unit test.
 *
 *  @see    Platform
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"

int unittestPlatform2() {
    int errors = 0;
    unsigned long data[] = {
        0xdeadbeef,
        0xc0edbabe,
        0xcafefeed,
        0xfacefade
    };
    char obuffer[] = { 'o', 'u', 't', 'p', 'u', 't', '_', 'b', '\n' };
    char ebuffer[] = { 'e', 'r', 'r', 'o', 'r', '_', 'b', '\n' };
    char message[80];
    Input* input;
    Output* output;
    Output* error;
    Dump* dump;
    Heap* heap;
    Logger* logger;
    char* pointer;

    input = platform_input();
    output = platform_output();
    error = platform_error();
    dump = platform_dump();
    heap = platform_heap();
    logger = platform_logger();

    print_f(output, "%s[%d]: begin\n", __FILE__, __LINE__);

    print_f(output, "%s[%d]: basics\n", __FILE__, __LINE__);

    print_f(output, "%s[%d]: platform=\"%s\"\n",
        __FILE__, __LINE__, platform_platform());
    print_f(output, "%s[%d]: target=\"%s\"\n",
        __FILE__, __LINE__, platform_target());
    print_f(output, "%s[%d]: host=\"%s\"\n",
        __FILE__, __LINE__, platform_host());
    print_f(output, "%s[%d]: identity=0x%llx\n",
        __FILE__, __LINE__, platform_identity());

    print_f(output, "%s[%d]: time\n", __FILE__, __LINE__);

    print_f(output, "%s[%d]: frequency=%llu\n",
        __FILE__, __LINE__, platform_frequency());
    print_f(output, "%s[%d]: time1=%llu\n",
        __FILE__, __LINE__, platform_time());
    print_f(output, "%s[%d]: elapsed1=%llu\n",
        __FILE__, __LINE__, platform_elapsed());
    print_f(output, "%s[%d]: yield1=%llu\n",
        __FILE__, __LINE__, platform_yield(platform_frequency(), 0));
    print_f(output, "%s[%d]: yield2=%llu\n",
        __FILE__, __LINE__, platform_yield(platform_frequency(), 1));
    print_f(output, "%s[%d]: time2=%llu\n",
        __FILE__, __LINE__, platform_time());
    print_f(output, "%s[%d]: elapsed2=%llu\n",
        __FILE__, __LINE__, platform_elapsed());

    print_f(output, "%s[%d]: errno\n", __FILE__, __LINE__);

    print_f(output, "%s[%d]: platform_errornumber=%p\n",
        __FILE__, __LINE__, platform_errornumber());
    print_f(output, "%s[%d]: platform_errormaximum=%d\n",
        __FILE__, __LINE__, platform_errormaximum());
    print_f(output, "%s[%d]: platform_errormessage=\"%s\"\n",
        __FILE__, __LINE__,
        platform_errormessage(EINVAL, message, sizeof(message)));

    print_f(output, "%s[%d]: writing to output\n",
        __FILE__, __LINE__);

    output_c(output, 'o');
    output_c(output, 'u');
    output_c(output, 't');
    output_c(output, 'p');
    output_c(output, 'u');
    output_c(output, 't');
    output_c(output, '_');
    output_c(output, 'c');
    output_c(output, '\n');

    output_s(output, "output_s\n");

    output_n(output, "output_n\n??????????", sizeof("output_n"));

    output_b(output, obuffer, sizeof(obuffer), sizeof(obuffer));

    print_f(output, "%s%c", "output_f", '\n');

    output_flush(output);

    print_f(output, "%s[%d]: writing to error\n",
        __FILE__, __LINE__);

    output_c(error, 'e');
    output_c(error, 'r');
    output_c(error, 'r');
    output_c(error, 'o');
    output_c(error, 'r');
    output_c(error, '_');
    output_c(error, 'c');
    output_c(error, '\n');

    output_s(error, "error_s\n");

    output_n(error, "error_n\n??????????", sizeof("error_n"));

    output_b(error, ebuffer, sizeof(ebuffer), sizeof(ebuffer));

    print_f(error, "%s%c", "error_f", '\n');

    output_flush(error);

    platform_printf("%s%c", "platform_printf", '\n');
    platform_errorf("%s%c", "platform_errorf", '\n');

    print_f(output, "%s[%d]: dump\n", __FILE__, __LINE__);

    dump_bytes(dump, data, sizeof(data));
    dump_words(dump, data, sizeof(data));

    print_f(output, "%s[%d]: heap\n", __FILE__, __LINE__);

    pointer = (char*)heap_malloc(heap, 16);
    pointer[0] = '\0';
    pointer[15] = '\7';
    pointer = (char*)heap_realloc(heap, pointer, 256);
    pointer[0] = '\0';
    pointer[255] = '\377';
    heap_free(heap, pointer);
    pointer = (char*)heap_calloc(heap, 128, 2);
    pointer[0] = '\0';
    pointer[255] = '\377';
    heap_free(heap, pointer);

    print_f(output, "%s[%d]: logger\n", __FILE__, __LINE__);

    logger_finest(logger, "%s", "logger");
    logger_finer(logger, "%s", "logger");
    logger_fine(logger, "%s", "logger");
    logger_trace(logger, "%s", "logger");
    logger_debug(logger, "%s", "logger");
    logger_information(logger, "%s", "logger");
    logger_configuration(logger, "%s", "logger");
    logger_notice(logger, "%s", "logger");
    logger_warning(logger, "%s", "logger");
    logger_error(logger, "%s", "logger");
    logger_severe(logger, "%s", "logger");
    logger_critical(logger, "%s", "logger");
    logger_alert(logger, "%s", "logger");
    logger_fatal(logger, "%s", "logger");
    logger_emergency(logger, "%s", "logger");
    logger_print(logger, "%s", "logger");

    print_f(output, "%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
