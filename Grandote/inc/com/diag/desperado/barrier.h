#ifndef _COM_DIAG_DESPERADO_BARRIER_H_
#define _COM_DIAG_DESPERADO_BARRIER_H_

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
 *  Defines a portable memory barrier interface.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/cxxcapi.h"


/**
 *  Defines an interface for a generic memory barrier, then
 *  tries to implement it in a platform and target independent
 *  manner. This is not a thread synchronization barrier. It is
 *  a memory fence to insure that memory writes are not reordered
 *  past it ("release" semantics) and memory reads are not reordered
 *  before it ("acquire" semantics)."
 *
 *  This function uses a POSIX mutex allocated on the stack solely for
 *  its side effect of calling the underlying platform's memory barrier
 *  mechanism (if any) solely as a side effect.  This is portable but
 *  expensive compared to compiler and target specific mechanisms using
 *  macros like smp_mb(), mb(), or barrier() that are defined to the
 *  appropriate assembler instructions, at least on Linux systems.
 *  You can try using those macros, but besides the portability issues,
 *  it is a real trick to get the native mechanisms to work under both C
 *  and C++, and under both the 2.4 kernel with gcc 3.3.2, and the
 *  2.6 kernel with gcc 3.3.4.
 *
 *  Note that this is expensive enough, you may be better off taking
 *  the simple, safe approach and just using a mutex to synchronize
 *  access to any data shared between threads, even on a uniprocessor
 *  system. However, it is not as expensive as it might seem, since
 *  the mutex is guaranteed never to block the calling thread.
 *
 *  I was interested in experimenting with a portable memory barrier
 *  implementation. Since I haven't figured out how to adequately unit
 *  test this, this must be considered highly experimental.
 *
 *  This is usable from either a C or C++ translation unit.
 *
 *  @see    desperado_native_barrier()
 *
 *  @see    Mutex
 *
 *  @see    CriticalSection
 *
 *  @see    ISO, <I>Information technology - Portable Operating System
 *          Interface (POSIX) - Part 1: System Application Program Interface
 *          (API) [C Language]</I>, International Organization for
 *          Standardization, ISO/IEC 9945-1:1996(E), 1996
 *
 *  @see    ISO, <I>Programming languages - C++</I>, International Organization
 *          for Standardization, ISO/IEC 14882:1998(E), 1998
 *
 *  @see    Wikipedia, "Memory barrier",
 *          http://en.wikipedia.org/wiki/Memory_barrier
 *
 *  @see    D. Bacon et al., <I>The "Double-Checked Locking Pattern is
 *          Broken" Declaration</I>,
 *          http://www.cs.umd.edu/~pugh/java/memoryModel/DoubleCheckedLocking.html
 *
 *  @see    S. Meyers at al., , "C++ and the Perils of Double-Checked
 *          Locking", <I>Dr. Dobb's Journal</I>, #362, July 2004 (part 1),
 *          #363, August 2004 (part 2)
 *
 *  @see    S. Meyers, "Double-Checked Locking, Threads, Compiler
 *          Optimizations, and More", 2004-04-17
 *
 *  @see    A. Alexandrescu et al., "Memory model for multithreaded C++",
 *          SG22 POSIX Advisory Group, WG21/N1680=J16/04-0120,
 *          2004-09-10
 *
 *  @see    M. Hill, "Multiprocessors Should Support Simple Memory-
 *          Consistency Models", <I>IEEE Computer</I>, August 1998
 *
 *  @see    S. Adve et al., "Shared Memory Consistency Models: A Tutorial",
 *          <I>IEEE Computer</I>, December 1996
 *
 *  @see    V. Morrison, "The DOTNET Memory Model", DOTNET Archives,
 *          March 2002, week 2, #6
 *
 *  @see    H. Boehm, "Threads Cannot be Implemented as a Library",
 *          Hewlett-Packard, HPL-2004-209, 2004-11-12
 *
 *  @see    H. Boehm, "Multithreaded Programming Issues for Application
 *          Programs", Hewlett-Packard, 2004
 *
 *  @return zero if successful, non-zero otherwise.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
CXXCAPI int desperado_portable_barrier(void);


#if !defined(desperado_native_barrier)
#   if defined(__GNUC__)
#       if defined(__GNUC_MINOR__)
#           if ((((__GNUC__)*1000)+(__GNUC_MINOR__))>=4001)
#               define desperado_native_barrier() (__sync_synchronize(), 0)
#           endif
#       endif
#   endif
#endif

#if !defined(desperado_native_barrier)
#   define desperado_native_barrier() desperado_portable_barrier()
#	warning Using desperado_portable_barrier!
#endif


/**
 *  Implements a memory barrier using the native mechanisms if such
 *  is available on the target and platform. May degenerate to calling
 *  the portable barrier function if it is not. A built-in memory
 *  fence was implemented in GCC starting in 4.1.
 *
 *  Since I haven't figured out how to adequately unit test this,
 *  this must be considered highly experimental.
 *
 *  This is usable from either a C or C++ translation unit.
 *
 *  @see    desperado_portable_barrier()
 *
 *  @see    R. Stallman et al., <I>Using the GCC Compiler Collection
 *          (GCC)</I>, GCC 4.1.1, GNU Press, October 2003
 *
 *  @return zero if successful, non-zero otherwise.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
CXXCINLINE int desperado_memory_barrier(void) {
    return desperado_native_barrier();
}


#if defined(DESPERADO_HAS_UNITTESTS)
/**
 *  Run the barrier unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestbarrier(void);
/**
 *  Run the barrier CXXCAPI unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestbarrier2(void);
#endif


#endif
