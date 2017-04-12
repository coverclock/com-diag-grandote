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
 *  Implements the Logger C API.
 *
 *  @see    Logger
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"


#define DESPERADO_LOGGER_API(_FUNCTION_, _LEVEL_) \
CXXCAPI ssize_t logger_##_FUNCTION_(Logger* logger, const char* format, ...) { \
    va_list ap; \
    va_start(ap, format); \
    ssize_t rc = logger->vlog(Logger::_LEVEL_, format, ap); \
    va_end(ap); \
    return rc; \
}


DESPERADO_LOGGER_API(   finest,         FINEST          )
DESPERADO_LOGGER_API(   finer,          FINER           )
DESPERADO_LOGGER_API(   fine,           FINE            )
DESPERADO_LOGGER_API(   trace,          TRACE           )
DESPERADO_LOGGER_API(   debug,          DEBUG           )
DESPERADO_LOGGER_API(   information,    INFORMATION     )
DESPERADO_LOGGER_API(   configuration,  CONFIGURATION   )
DESPERADO_LOGGER_API(   notice,         NOTICE          )
DESPERADO_LOGGER_API(   warning,        WARNING         )
DESPERADO_LOGGER_API(   error,          ERROR           )
DESPERADO_LOGGER_API(   severe,         SEVERE          )
DESPERADO_LOGGER_API(   critical,       CRITICAL        )
DESPERADO_LOGGER_API(   alert,          ALERT           )
DESPERADO_LOGGER_API(   fatal,          FATAL           )
DESPERADO_LOGGER_API(   emergency,      EMERGENCY       )
DESPERADO_LOGGER_API(   print,          PRINT           )
