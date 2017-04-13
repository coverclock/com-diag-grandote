/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * The purpose of this translation unit is to embed the release string
 * inside the library or shared object. The object module will be statically
 * linked into an application only if the translation unit makes explicit
 * references to the storage here as external references.
 */

#include "com/diag/grandote/release.h"

const char COM_DIAG_GRANDOTE_RELEASE_KEYWORD[] = "COM_DIAG_GRANDOTE_RELEASE=" COM_DIAG_GRANDOTE_RELEASE;
const char * COM_DIAG_GRANDOTE_RELEASE_VALUE = &COM_DIAG_GRANDOTE_RELEASE_KEYWORD[sizeof("COM_DIAG_GRANDOTE_RELEASE=") - 1];
