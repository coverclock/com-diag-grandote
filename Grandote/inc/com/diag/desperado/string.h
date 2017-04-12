#ifndef _COM_DIAG_DESPERADO_STRING_H_
#define _COM_DIAG_DESPERADO_STRING_H_

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
 *  Defines the Desperado string functions for those platforms
 *  which do not implement these common GNU and BSD extensions.
 * 
 *  This can be included from either a C or a C++ translation unit.
 * 
 *  These functions have not all be adequately unit tested.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#if defined(__cplusplus)
#   include <cstring>
#   include <string>
#else
#   include <string.h>
#endif
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/cxxcapi.h"


/*
 *  Define the prototypes for the Desperado string functions. These
 *  functions are available no matter what platform we are building
 *  for, but they might be redundant, duplicating functionality that
 *  the platform already has.
 */
 

/**
 *  Return the length of a character string, or a maximum value
 *  if the NUL terminator is not encountered first.
 *
 *  GNU defines this as an extension, and for most platforms as
 *  an assembly language inline function.
 *
 *  @param  s           points to the input string.
 *
 *  @param  maxlen      is the maximum possible length.
 *
 *  @return the string length not including its terminating NUL,
 *          or the count, whichever is smaller.
 */
CXXCAPI size_t desperado_strnlen(const char* s, size_t maxlen);


/**
 *  Implements the OpenBSD strlcpy function that is used similarly to
 *  strcpy but guarantees the the destination buffer is NUL terminated.
 *
 *  @param  dst     points to the destination buffer.
 *
 *  @param  src     points to the source buffer.
 *
 *  @param  size    is the size of the destination buffer in bytes.
 *
 *  @return the number of bytes not copied into the destination buffer
 *          because it was not large enough.
 *
 *  @see    J. Viega, M. Messier, "Input Validation in C and C++",
 *          O'Reilly Network, 2003
 *
 *  @see    T. Miller, T. de Raadt, "strlcpy and strlcat - Consistent,
 *          Safe, String Copy and Concatenation", Proceedings of the
 *          1999 USENIX Annual Technical Conference, 1999
 */
CXXCAPI size_t desperado_strlcpy(char* dst, const char* src, size_t size);


/**
 *  Implements the OpenBSD strlcat function that is used similarly to
 *  strcat but guarantees the the destination buffer is NUL terminated.
 *
 *  @param  dst     points to the destination buffer.
 *
 *  @param  src     points to the source buffer.
 *
 *  @param  size    is the size of the destination buffer in bytes.
 *
 *  @return the number of bytes not concatenated onto the destination buffer
 *          because it was not large enough.
 *
 *  @see    J. Viega, M. Messier, "Input Validation in C and C++",
 *          O'Reilly Network, 2003
 *
 *  @see    T. Miller, T. de Raadt, "strlcpy and strlcat - Consistent,
 *          Safe, String Copy and Concatenation", Proceedings of the
 *          1999 USENIX Annual Technical Conference, 1999
 */
CXXCAPI size_t desperado_strlcat(char* dst, const char* src, size_t size);


/**
 *  Search for the first occurance of a character in a string of limited length.
 *
 *  Most Linux systems defined strnchr(9) but it is not part of the
 *  standard (yet).
 *
 *  @param  s           points to the string to be searched.
 *
 *  @param  count       is the maximum number of characters to be searched.
 *
 *  @param  c           is the character to search for.
 *
 *  @return a pointer to the first occurance of the desired character
 *          from the left or 0 if not found.
 */
CXXCAPI char* desperado_strnchr(const char* s, size_t count, int c);


/**
 *  Search for the last occurance of a character in a string of limited length.
 *
 *  You would expect there to be a strnrchr() function.
 *
 *  @param  s           points to the string to be searched.
 *
 *  @param  count       is the maximum number of characters to be searched.
 *
 *  @param  c           is the character to search for.
 *
 *  @return a pointer to the first occurance of the desired character
 *          from the right or 0 if not found.
 */
CXXCAPI char* desperado_strnrchr(const char* s, size_t count, int c);


/*
 *  Try to determine what functions we need on the platform for which
 *  we are building.
 */
 

#if defined(__CYGWIN__)
#   define DESPERADO_NEEDS_STRNCHR
#elif defined(__CYGWIN32__)
#   define DESPERADO_NEEDS_STRNCHR
#elif defined(__USE_GNU)
#   define DESPERADO_NEEDS_STRNCHR
#elif defined(__OpenBSD__)
#else
#   if !defined(__HAVE_ARCH_STRNLEN)
#       define DESPERADO_NEEDS_STRNLEN
#   endif
#   if !defined(__HAVE_ARCH_STRLCPY)
#       define DESPERADO_NEEDS_STRLCPY
#   endif
#   if !defined(__HAVE_ARCH_STRLCAT)
#       define DESPERADO_NEEDS_STRLCAT
#   endif
#   define DESPERADO_NEEDS_STRNCHR
#endif
#define DESPERADO_NEEDS_STRNRCHR


/*
 *  Define the functions that we think we need. Note that
 *  since these are not part of the ANSI standard, we leave
 *  them in the global namespace instead of the std namespace.
 */


#if defined(DESPERADO_NEEDS_STRNLEN)

/**
 *  Return the length of a character string, or a maximum value
 *  if the NUL terminator is not encountered first.
 *
 *  GNU defines this as an extension, and for most platforms as
 *  an assembly language inline function.
 *
 *  @param  s           points to the input string.
 *
 *  @param  maxlen      is the maximum possible length.
 *
 *  @return the string length not including its terminating NUL,
 *          or the count, whichever is smaller.
 */
CXXCINLINE size_t strnlen(const char* s, size_t maxlen) {
    return desperado_strnlen(s, maxlen);
}

#endif


#if defined(DESPERADO_NEEDS_STRLCPY)

/**
 *  Implements the OpenBSD strlcpy function that is used similarly to
 *  strcpy but guarantees the the destination buffer is NUL terminated.
 *
 *  @param  dst     points to the destination buffer.
 *
 *  @param  src     points to the source buffer.
 *
 *  @param  count   is the size of the destination buffer in bytes.
 *
 *  @return the number of bytes not copied into the destination buffer
 *          because it was not large enough.
 *
 *  @see    J. Viega, M. Messier, "Input Validation in C and C++",
 *          O'Reilly Network, 2003
 *
 *  @see    T. Miller, T. de Raadt, "strlcpy and strlcat - Consistent,
 *          Safe, String Copy and Concatenation", Proceedings of the
 *          1999 USENIX Annual Technical Conference, 1999
 */
CXXCINLINE size_t strlcpy(char* dst, const char* src, size_t count) {
    return desperado_strlcpy(dst, src, count);
}

#endif


#if defined(DESPERADO_NEEDS_STRLCAT)

/**
 *  Implements the OpenBSD strlcat function that is used similarly to
 *  strcat but guarantees the the destination buffer is NUL terminated.
 *
 *  @param  dst     points to the destination buffer.
 *
 *  @param  src     points to the source buffer.
 *
 *  @param  count   is the size of the destination buffer in bytes.
 *
 *  @return the number of bytes not concatenated onto the destination buffer
 *          because it was not large enough.
 *
 *  @see    J. Viega, M. Messier, "Input Validation in C and C++",
 *          O'Reilly Network, 2003
 *
 *  @see    T. Miller, T. de Raadt, "strlcpy and strlcat - Consistent,
 *          Safe, String Copy and Concatenation", Proceedings of the
 *          1999 USENIX Annual Technical Conference, 1999
 */
CXXCINLINE size_t strlcat(char* dst, const char* src, size_t count) {
    return desperado_strlcat(dst, src, count);
}

#endif


#if defined(DESPERADO_NEEDS_STRNCHR)

/**
 *  Search for the first occurance of a character in a string of limited length.
 *
 *  @param  s           points to the string to be searched.
 *
 *  @param  count       is the maximum number of characters to be searched.
 *
 *  @param  c           is the character to search for.
 *
 *  @return a pointer to the first occurance of the desired character
 *          from the left or 0 if not found.
 */
CXXCINLINE char* strnchr(const char* s, size_t count, int c) {
    return desperado_strnchr(s, count, c);
}

#endif


#if defined(DESPERADO_NEEDS_STRNRCHR)

/**
 *  Search for the last occurance of a character in a string of limited length.
 *
 *  @param  s           points to the string to be searched.
 *
 *  @param  count       is the maximum number of characters to be searched.
 *
 *  @param  c           is the character to search for.
 *
 *  @return a pointer to the first occurance of the desired character
 *          from the right or 0 if not found.
 */
CXXCINLINE char* strnrchr(const char* s, size_t count, int c) {
    return desperado_strnrchr(s, count, c);
}

#endif

#if defined(__cplusplus)

/**
 *	Return a pointer to a character array suitable for printing
 *	using a %s print format.
 *
 *	@param cp			points to a character array.
 *
 *	@return a pointer to a character array.
 */
inline const char* _s(const char* cp) { return cp; }

/**
 *	Return a pointer to a character array suitable for printing
 *	using a %s print format.
 *
 *	@param sr			refers to a C++ string.
 *
 *	@return a pointer to a character array.
 */
inline const char* _s(const std::string & sr) { return sr.c_str(); }

/**
 *	Return a pointer to a character array suitable for printing
 *	using a %s print format.
 *
 *	@param sp			points to a C++ string.
 *
 *	@return a pointer to a character array.
 */
inline const char* _s(const std::string * sp) { return sp->c_str(); }

#endif


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the string unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unitteststring(void);
#endif


#endif
