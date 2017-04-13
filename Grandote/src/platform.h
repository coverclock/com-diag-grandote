/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_GRANDOTE_PLATFORM_H_PRIVATE_
#define _COM_DIAG_GRANDOTE_PLATFORM_H_PRIVATE_

/**
 *  @file
 *
 * Copyright 2017 Digital Aggregates Corporation<BR>
 * Licensed under the terms in LICENSE.txt.<BR>
 * mailto:coverclock@diag.com<BR>
 */

#include "com/diag/diminuto/diminuto_platform.h"


#if defined(COM_DIAG_DIMINUTO_PLATFORM_GLIBC)
#   define GRANDOTE_PLATFORM_IS_Diminuto
#   define GRANDOTE_PLATFORM_NAME COM_DIAG_DIMINUTO_PLATFORM
#else
#   warning COM_DIAG_DIMINUTO_PLATFORM not recognized!!
#   define GRANDOTE_PLATFORM_IS_Linux
#   define GRANDOTE_PLATFORM_NAME "Linux"
#endif

#endif
