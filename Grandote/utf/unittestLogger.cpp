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
 *  Implements the Logger unit test.
 *
 *  @see    Logger
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/grandote/UnitTest.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/Logger.h"
#include "com/diag/grandote/Logger.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/Print.h"
#include "com/diag/grandote/Print.h"
#include "com/diag/grandote/LogOutput.h"
#include "com/diag/grandote/LogOutput.h"
#include "com/diag/grandote/Grandote.h"

static int leveltest(const char* ff, ...) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;
    va_list ap;
    char buffer[Output::minimum_buffer_size];
    Logger logger;
    va_start(ap, ff);
    for (int level = 0; Logger::PRINT >= level; ++level) {
        ssize_t rc = logger.format(buffer, sizeof(buffer),
                            static_cast<Logger::Level>(level),
                            ff, ap);
        if (rc >= static_cast<ssize_t>(sizeof(buffer))) {
            errorf("%s[%d]: (%zd>=%lu)!\n",
                __FILE__, __LINE__, rc, sizeof(buffer));
            ++errors;
        }
        size_t length = ::strnlen(buffer, sizeof(buffer));
        if (length >= sizeof(buffer)) {
            errorf("%s[%d]: (%lu>=%lu)!\n",
                __FILE__, __LINE__, length, sizeof(buffer));
            ++errors;
        }
        if (static_cast<ssize_t>(length) != rc) {
            errorf("%s[%d]: (%lu>=%zd)!\n",
                __FILE__, __LINE__, length, rc);
            ++errors;
        }
        size_t claimed;
        logger.level(buffer, sizeof(buffer), claimed);
        if (claimed != static_cast<size_t>(level)) {
            errorf("%s[%d]: (%u!=%d)!\n",
                __FILE__, __LINE__, claimed, level);
            ++errors;
        }
        printf("%s[%d]: \"%s\"[%lu:%zd:%lu](%d:%u)\n",
                __FILE__, __LINE__, buffer, sizeof(buffer), rc, length,
                level, claimed);
    }
    va_end(ap);
    return errors;
}

static Logger staticLogger;

CXXCAPI int unittestLogger(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticLogger.show();

    printf("%s[%d]: level\n", __FILE__, __LINE__);

    errors += leveltest("%s[%d]", __FILE__, __LINE__);

    printf("%s[%d]: default\n", __FILE__, __LINE__);

    ssize_t rc;

    Logger logger1;

    logger1.show();

#define GRANDOTE_LOGGER_TEST_1(_FUNCTION_) \
    rc = logger1._FUNCTION_("%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger1", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    GRANDOTE_LOGGER_TEST_1(finest)
    GRANDOTE_LOGGER_TEST_1(finer)
    GRANDOTE_LOGGER_TEST_1(fine)
    GRANDOTE_LOGGER_TEST_1(trace)
    GRANDOTE_LOGGER_TEST_1(debug)
    GRANDOTE_LOGGER_TEST_1(information)
    GRANDOTE_LOGGER_TEST_1(configuration)
    GRANDOTE_LOGGER_TEST_1(notice)
    GRANDOTE_LOGGER_TEST_1(warning)
    GRANDOTE_LOGGER_TEST_1(error)
    GRANDOTE_LOGGER_TEST_1(severe)
    GRANDOTE_LOGGER_TEST_1(critical)
    GRANDOTE_LOGGER_TEST_1(alert)
    GRANDOTE_LOGGER_TEST_1(fatal)
    GRANDOTE_LOGGER_TEST_1(emergency)
    GRANDOTE_LOGGER_TEST_1(print)

    printf("%s[%d]: platform\n", __FILE__, __LINE__);

    Logger& logger2 = Platform::instance().logger();

    logger2.show();

#define GRANDOTE_LOGGER_TEST_2(_FUNCTION_) \
    rc = logger2._FUNCTION_("%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger2", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    GRANDOTE_LOGGER_TEST_2(finest)
    GRANDOTE_LOGGER_TEST_2(finer)
    GRANDOTE_LOGGER_TEST_2(fine)
    GRANDOTE_LOGGER_TEST_2(trace)
    GRANDOTE_LOGGER_TEST_2(debug)
    GRANDOTE_LOGGER_TEST_2(information)
    GRANDOTE_LOGGER_TEST_2(configuration)
    GRANDOTE_LOGGER_TEST_2(notice)
    GRANDOTE_LOGGER_TEST_2(warning)
    GRANDOTE_LOGGER_TEST_2(error)
    GRANDOTE_LOGGER_TEST_2(severe)
    GRANDOTE_LOGGER_TEST_2(critical)
    GRANDOTE_LOGGER_TEST_2(alert)
    GRANDOTE_LOGGER_TEST_2(fatal)
    GRANDOTE_LOGGER_TEST_2(emergency)
    GRANDOTE_LOGGER_TEST_2(print)

    printf("%s[%d]: output\n", __FILE__, __LINE__);

    Logger logger3(Platform::instance().output());

    logger3.show();

#define GRANDOTE_LOGGER_TEST_3(_FUNCTION_) \
    rc = logger2._FUNCTION_("%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger3", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    GRANDOTE_LOGGER_TEST_3(finest)
    GRANDOTE_LOGGER_TEST_3(finer)
    GRANDOTE_LOGGER_TEST_3(fine)
    GRANDOTE_LOGGER_TEST_3(trace)
    GRANDOTE_LOGGER_TEST_3(debug)
    GRANDOTE_LOGGER_TEST_3(information)
    GRANDOTE_LOGGER_TEST_3(configuration)
    GRANDOTE_LOGGER_TEST_3(notice)
    GRANDOTE_LOGGER_TEST_3(warning)
    GRANDOTE_LOGGER_TEST_3(error)
    GRANDOTE_LOGGER_TEST_3(severe)
    GRANDOTE_LOGGER_TEST_3(critical)
    GRANDOTE_LOGGER_TEST_3(alert)
    GRANDOTE_LOGGER_TEST_3(fatal)
    GRANDOTE_LOGGER_TEST_3(emergency)
    GRANDOTE_LOGGER_TEST_3(print)

    printf("%s[%d]: cxxcapi\n", __FILE__, __LINE__);

    Logger* logger4 = platform_logger();

    logger4->show();

#define GRANDOTE_LOGGER_TEST_4(_FUNCTION_) \
    rc = logger_##_FUNCTION_(logger4, "%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger4", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    GRANDOTE_LOGGER_TEST_4(finest)
    GRANDOTE_LOGGER_TEST_4(finer)
    GRANDOTE_LOGGER_TEST_4(fine)
    GRANDOTE_LOGGER_TEST_4(trace)
    GRANDOTE_LOGGER_TEST_4(debug)
    GRANDOTE_LOGGER_TEST_4(information)
    GRANDOTE_LOGGER_TEST_4(configuration)
    GRANDOTE_LOGGER_TEST_4(notice)
    GRANDOTE_LOGGER_TEST_4(warning)
    GRANDOTE_LOGGER_TEST_4(error)
    GRANDOTE_LOGGER_TEST_4(severe)
    GRANDOTE_LOGGER_TEST_4(critical)
    GRANDOTE_LOGGER_TEST_4(alert)
    GRANDOTE_LOGGER_TEST_4(fatal)
    GRANDOTE_LOGGER_TEST_4(emergency)
    GRANDOTE_LOGGER_TEST_4(print)

    printf("%s[%d]: log\n", __FILE__, __LINE__);

    Logger logger5;

    logger5.show();

#define GRANDOTE_LOGGER_TEST_5(_LEVEL_) \
    rc = logger5.log(Logger::_LEVEL_, "%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger5", #_LEVEL_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    GRANDOTE_LOGGER_TEST_5(FINEST)
    GRANDOTE_LOGGER_TEST_5(FINER)
    GRANDOTE_LOGGER_TEST_5(FINE)
    GRANDOTE_LOGGER_TEST_5(TRACE)
    GRANDOTE_LOGGER_TEST_5(DEBUG)
    GRANDOTE_LOGGER_TEST_5(INFORMATION)
    GRANDOTE_LOGGER_TEST_5(CONFIGURATION)
    GRANDOTE_LOGGER_TEST_5(NOTICE)
    GRANDOTE_LOGGER_TEST_5(WARNING)
    GRANDOTE_LOGGER_TEST_5(ERROR)
    GRANDOTE_LOGGER_TEST_5(SEVERE)
    GRANDOTE_LOGGER_TEST_5(CRITICAL)
    GRANDOTE_LOGGER_TEST_5(ALERT)
    GRANDOTE_LOGGER_TEST_5(FATAL)
    GRANDOTE_LOGGER_TEST_5(EMERGENCY)
    GRANDOTE_LOGGER_TEST_5(PRINT)

    Logger logger7;

#define KERN_EMERG      "<0>"
#define KERN_ALERT      "<1>"
#define KERN_CRIT       "<2>"
#define KERN_ERR        "<3>"
#define KERN_WARNING    "<4>"
#define KERN_NOTICE     "<5>"
#define KERN_INFO       "<6>"
#define KERN_DEBUG      "<7>"

#define GRANDOTE_LOGGER_TEST_7(_LEVEL_, _KERNEL_) { \
    const char * message = _KERNEL_ #_KERNEL_; \
    size_t level; \
    logger7.level(message, std::strlen(message), level); \
    if (static_cast<size_t>(Logger::_LEVEL_) != level) { \
        errorf("%s[%d]: (%d!=%u)!\n", __FILE__, __LINE__, Logger::_LEVEL_, level); \
        ++errors; \
    } else { \
        printf("%s[%d]: \"%s\" %u %s\n", __FILE__, __LINE__, message, level, Logger::labels[level]); \
    } \
}

    GRANDOTE_LOGGER_TEST_7(EMERGENCY,		KERN_EMERG);
    GRANDOTE_LOGGER_TEST_7(ALERT,	    	KERN_ALERT);
    GRANDOTE_LOGGER_TEST_7(CRITICAL,		KERN_CRIT);
    GRANDOTE_LOGGER_TEST_7(ERROR,	    	KERN_ERR);
    GRANDOTE_LOGGER_TEST_7(WARNING,		KERN_WARNING);
    GRANDOTE_LOGGER_TEST_7(NOTICE,	    	KERN_NOTICE);
    GRANDOTE_LOGGER_TEST_7(INFORMATION,	KERN_INFO);
    GRANDOTE_LOGGER_TEST_7(DEBUG,		    KERN_DEBUG);

    FileOutput file(stderr);
    Logger logger8;
    LogOutput output(file);
    Logger newlogger8(output);
    logger8 = newlogger8;

    logger8.show();

#define GRANDOTE_LOGGER_TEST_8(_FUNCTION_) \
    rc = logger8._FUNCTION_("%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger8", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    GRANDOTE_LOGGER_TEST_8(finest)
    GRANDOTE_LOGGER_TEST_8(finer)
    GRANDOTE_LOGGER_TEST_8(fine)
    GRANDOTE_LOGGER_TEST_8(trace)
    GRANDOTE_LOGGER_TEST_8(debug)
    GRANDOTE_LOGGER_TEST_8(information)
    GRANDOTE_LOGGER_TEST_8(configuration)
    GRANDOTE_LOGGER_TEST_8(notice)
    GRANDOTE_LOGGER_TEST_8(warning)
    GRANDOTE_LOGGER_TEST_8(error)
    GRANDOTE_LOGGER_TEST_8(severe)
    GRANDOTE_LOGGER_TEST_8(critical)
    GRANDOTE_LOGGER_TEST_8(alert)
    GRANDOTE_LOGGER_TEST_8(fatal)
    GRANDOTE_LOGGER_TEST_8(emergency)
    GRANDOTE_LOGGER_TEST_8(print)

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
