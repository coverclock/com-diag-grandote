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


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/Desperado.h"

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

#define DESPERADO_LOGGER_TEST_1(_FUNCTION_) \
    rc = logger1._FUNCTION_("%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger1", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    DESPERADO_LOGGER_TEST_1(finest)
    DESPERADO_LOGGER_TEST_1(finer)
    DESPERADO_LOGGER_TEST_1(fine)
    DESPERADO_LOGGER_TEST_1(trace)
    DESPERADO_LOGGER_TEST_1(debug)
    DESPERADO_LOGGER_TEST_1(information)
    DESPERADO_LOGGER_TEST_1(configuration)
    DESPERADO_LOGGER_TEST_1(notice)
    DESPERADO_LOGGER_TEST_1(warning)
    DESPERADO_LOGGER_TEST_1(error)
    DESPERADO_LOGGER_TEST_1(severe)
    DESPERADO_LOGGER_TEST_1(critical)
    DESPERADO_LOGGER_TEST_1(alert)
    DESPERADO_LOGGER_TEST_1(fatal)
    DESPERADO_LOGGER_TEST_1(emergency)
    DESPERADO_LOGGER_TEST_1(print)

    printf("%s[%d]: platform\n", __FILE__, __LINE__);

    Logger& logger2 = Platform::instance().logger();

    logger2.show();

#define DESPERADO_LOGGER_TEST_2(_FUNCTION_) \
    rc = logger2._FUNCTION_("%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger2", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    DESPERADO_LOGGER_TEST_2(finest)
    DESPERADO_LOGGER_TEST_2(finer)
    DESPERADO_LOGGER_TEST_2(fine)
    DESPERADO_LOGGER_TEST_2(trace)
    DESPERADO_LOGGER_TEST_2(debug)
    DESPERADO_LOGGER_TEST_2(information)
    DESPERADO_LOGGER_TEST_2(configuration)
    DESPERADO_LOGGER_TEST_2(notice)
    DESPERADO_LOGGER_TEST_2(warning)
    DESPERADO_LOGGER_TEST_2(error)
    DESPERADO_LOGGER_TEST_2(severe)
    DESPERADO_LOGGER_TEST_2(critical)
    DESPERADO_LOGGER_TEST_2(alert)
    DESPERADO_LOGGER_TEST_2(fatal)
    DESPERADO_LOGGER_TEST_2(emergency)
    DESPERADO_LOGGER_TEST_2(print)

    printf("%s[%d]: output\n", __FILE__, __LINE__);

    Logger logger3(Platform::instance().output());

    logger3.show();

#define DESPERADO_LOGGER_TEST_3(_FUNCTION_) \
    rc = logger2._FUNCTION_("%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger3", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    DESPERADO_LOGGER_TEST_3(finest)
    DESPERADO_LOGGER_TEST_3(finer)
    DESPERADO_LOGGER_TEST_3(fine)
    DESPERADO_LOGGER_TEST_3(trace)
    DESPERADO_LOGGER_TEST_3(debug)
    DESPERADO_LOGGER_TEST_3(information)
    DESPERADO_LOGGER_TEST_3(configuration)
    DESPERADO_LOGGER_TEST_3(notice)
    DESPERADO_LOGGER_TEST_3(warning)
    DESPERADO_LOGGER_TEST_3(error)
    DESPERADO_LOGGER_TEST_3(severe)
    DESPERADO_LOGGER_TEST_3(critical)
    DESPERADO_LOGGER_TEST_3(alert)
    DESPERADO_LOGGER_TEST_3(fatal)
    DESPERADO_LOGGER_TEST_3(emergency)
    DESPERADO_LOGGER_TEST_3(print)

    printf("%s[%d]: cxxcapi\n", __FILE__, __LINE__);

    Logger* logger4 = platform_logger();

    logger4->show();

#define DESPERADO_LOGGER_TEST_4(_FUNCTION_) \
    rc = logger_##_FUNCTION_(logger4, "%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger4", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    DESPERADO_LOGGER_TEST_4(finest)
    DESPERADO_LOGGER_TEST_4(finer)
    DESPERADO_LOGGER_TEST_4(fine)
    DESPERADO_LOGGER_TEST_4(trace)
    DESPERADO_LOGGER_TEST_4(debug)
    DESPERADO_LOGGER_TEST_4(information)
    DESPERADO_LOGGER_TEST_4(configuration)
    DESPERADO_LOGGER_TEST_4(notice)
    DESPERADO_LOGGER_TEST_4(warning)
    DESPERADO_LOGGER_TEST_4(error)
    DESPERADO_LOGGER_TEST_4(severe)
    DESPERADO_LOGGER_TEST_4(critical)
    DESPERADO_LOGGER_TEST_4(alert)
    DESPERADO_LOGGER_TEST_4(fatal)
    DESPERADO_LOGGER_TEST_4(emergency)
    DESPERADO_LOGGER_TEST_4(print)

    printf("%s[%d]: log\n", __FILE__, __LINE__);

    Logger logger5;

    logger5.show();

#define DESPERADO_LOGGER_TEST_5(_LEVEL_) \
    rc = logger5.log(Logger::_LEVEL_, "%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger5", #_LEVEL_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    DESPERADO_LOGGER_TEST_5(FINEST)
    DESPERADO_LOGGER_TEST_5(FINER)
    DESPERADO_LOGGER_TEST_5(FINE)
    DESPERADO_LOGGER_TEST_5(TRACE)
    DESPERADO_LOGGER_TEST_5(DEBUG)
    DESPERADO_LOGGER_TEST_5(INFORMATION)
    DESPERADO_LOGGER_TEST_5(CONFIGURATION)
    DESPERADO_LOGGER_TEST_5(NOTICE)
    DESPERADO_LOGGER_TEST_5(WARNING)
    DESPERADO_LOGGER_TEST_5(ERROR)
    DESPERADO_LOGGER_TEST_5(SEVERE)
    DESPERADO_LOGGER_TEST_5(CRITICAL)
    DESPERADO_LOGGER_TEST_5(ALERT)
    DESPERADO_LOGGER_TEST_5(FATAL)
    DESPERADO_LOGGER_TEST_5(EMERGENCY)
    DESPERADO_LOGGER_TEST_5(PRINT)

    Logger logger7;

#define KERN_EMERG      "<0>"
#define KERN_ALERT      "<1>"
#define KERN_CRIT       "<2>"
#define KERN_ERR        "<3>"
#define KERN_WARNING    "<4>"
#define KERN_NOTICE     "<5>"
#define KERN_INFO       "<6>"
#define KERN_DEBUG      "<7>"

#define DESPERADO_LOGGER_TEST_7(_LEVEL_, _KERNEL_) { \
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

    DESPERADO_LOGGER_TEST_7(EMERGENCY,		KERN_EMERG);
    DESPERADO_LOGGER_TEST_7(ALERT,	    	KERN_ALERT);
    DESPERADO_LOGGER_TEST_7(CRITICAL,		KERN_CRIT);
    DESPERADO_LOGGER_TEST_7(ERROR,	    	KERN_ERR);
    DESPERADO_LOGGER_TEST_7(WARNING,		KERN_WARNING);
    DESPERADO_LOGGER_TEST_7(NOTICE,	    	KERN_NOTICE);
    DESPERADO_LOGGER_TEST_7(INFORMATION,	KERN_INFO);
    DESPERADO_LOGGER_TEST_7(DEBUG,		    KERN_DEBUG);

    FileOutput file(stderr);
    Logger logger8;
    LogOutput output(file);
    Logger newlogger8(output);
    logger8 = newlogger8;

    logger8.show();

#define DESPERADO_LOGGER_TEST_8(_FUNCTION_) \
    rc = logger8._FUNCTION_("%s[%d]: %s %s\n", \
        __FILE__, __LINE__, "logger8", #_FUNCTION_); \
    if (0 >= rc) { \
        errorf("%s[%d]: (%d>%d)!\n", \
            __FILE__, __LINE__, 0, rc); \
        ++errors; \
    }

    DESPERADO_LOGGER_TEST_8(finest)
    DESPERADO_LOGGER_TEST_8(finer)
    DESPERADO_LOGGER_TEST_8(fine)
    DESPERADO_LOGGER_TEST_8(trace)
    DESPERADO_LOGGER_TEST_8(debug)
    DESPERADO_LOGGER_TEST_8(information)
    DESPERADO_LOGGER_TEST_8(configuration)
    DESPERADO_LOGGER_TEST_8(notice)
    DESPERADO_LOGGER_TEST_8(warning)
    DESPERADO_LOGGER_TEST_8(error)
    DESPERADO_LOGGER_TEST_8(severe)
    DESPERADO_LOGGER_TEST_8(critical)
    DESPERADO_LOGGER_TEST_8(alert)
    DESPERADO_LOGGER_TEST_8(fatal)
    DESPERADO_LOGGER_TEST_8(emergency)
    DESPERADO_LOGGER_TEST_8(print)

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
