/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_SET_
#define _H_COM_DIAG_DESPERADO_SET_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/stdlib.h"

namespace com {
namespace diag {
namespace desperado {

/**
 * Gets a C string, typically to use as an argument, from a provided C string,
 * or an empty string. Typically used in constructor initialization lists or
 * settor methods to construct C++ strings.
 * @param arg points to a provided value.
 * @return a pointer to the resulting value or an empty C string if none.
 */
inline const char * set(const char * arg) {
	return (arg != 0) ? arg : "";
}

/**
 * Gets a C string, typically to use as an argument, from a provided C string,
 * or from a default C string. Typically used in constructor initialization
 * lists or settor methods to construct C++ strings.
 * @param arg points to a provided value.
 * @param def points to a default value.
 * @return a pointer to the resulting value or an empty C string if none.
 */
inline const char * set(const char * arg, const char * def) {
	return (arg != 0) ? arg : (def != 0) ? def : "";
}

/**
 * Gets a C string, typically to use as an argument, from a provided C string,
 * from a named environmental variable, or from a default C string. Typically
 * used in constructor initialization lists or settor methods to construct C++
 * strings.
 * @param arg points to a provided value.
 * @param nam points to an environmental variable name.
 * @param def points to a default value.
 * @return a pointer to the resulting value or an empty C string if none.
 */
inline const char * set(const char * arg, const char * nam, const char * def) {
	const char * val;
	return (arg != 0) ? arg : ((nam != 0) && ((val = std::getenv(nam)) != 0)) ? val : (def != 0) ? def : "";
}

/**
 * Gets a C string from a C++ string is the latter is not empty, NULL otherwise.
 * Typically used in translating C++ strings to C strings for libs3 functions.
 *
 * @param str refers to a C++ string.
 * @return a C string or NULL.
 */
inline const char * set(const std::string & str) {
	return str.empty() ? 0 : str.c_str();
}

}
}
}

#endif
