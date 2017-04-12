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
 *  Implements the Fifo unit test.
 *
 *  @see    Fifo
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Fifo.h"
#include "com/diag/desperado/Fifo.h"
#include "com/diag/desperado/NewFifo.h"
#include "com/diag/desperado/NewFifo.h"
#include "com/diag/desperado/FifoType.h"
#include "com/diag/desperado/FifoType.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Desperado.h"

struct Datum { uint32_t one; uint32_t two; };

template class Fifo<Datum>;
template class Fifo<char>;
template class Fifo<int>;
template class Fifo<uint16_t>;
template class Fifo<uint32_t>;
template class Fifo<uint64_t>;
template class Fifo<uint8_t>;
template class FifoType<Datum, 25>;
template class FifoType<int, 25>;
template class NewFifo<int>;
template class NewFifo<uint16_t>;
template class NewFifo<uint64_t>;

static Fifo<int> staticFifo;
static NewFifo<int> staticNewFifo;
static FifoType<int, 25> staticFifoType;

CXXCAPI int unittestFifo(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticFifo.show();
    ::staticNewFifo.show();
    ::staticFifoType.show();

    static const unsigned int maximum = 25;
    static const unsigned int count = 16;

    printf("%s[%d]: construction\n", __FILE__, __LINE__);

    Datum datums[maximum] = { { 0, 0 } };
    Fifo<Datum> fifo(datums, countof(datums));
    fifo.show();
    Datum datuminstance;
    bool success = fifo.peek(datuminstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = fifo.remove(datuminstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    uint32_t words[16] = { 0 };
    Fifo<uint32_t> null1(0, countof(words));
    null1.show();
    uint32_t wordinstance;
    success = null1.peek(wordinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = null1.remove(wordinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = null1.insert(wordinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    char bytes[32] = { 0 };
    Fifo<char> null2(bytes, 0);
    null2.show();
    char byteinstance;
    success = null2.peek(byteinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = null2.remove(byteinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = null2.insert(byteinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    Fifo<uint8_t> null3;
    null3.show();
    uint8_t octetinstance;
    success = null3.peek(octetinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = null3.remove(octetinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = null3.insert(octetinstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    NewFifo<uint64_t> null4;
    null4.show();
    uint64_t longlonginstance;
    success = null4.peek(longlonginstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = null4.remove(longlonginstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = null4.insert(longlonginstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    FifoType<Datum, countof(datums)> typefifo;
    typefifo.show();
    success = typefifo.peek(datuminstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }
    success = typefifo.remove(datuminstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    printf("%s[%d]: show empty\n", __FILE__, __LINE__);

    fifo.show(1);

    printf("%s[%d]: fill\n", __FILE__, __LINE__);

    for (unsigned int jj = 0; count > jj; ++jj) {
        datuminstance.one = 0xa5a5a5a5;
        datuminstance.two = jj + 1;
        success = fifo.insert(datuminstance);
        if (true != success) {
            errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, true, success);
            ++errors;
        }
    }

    datuminstance.one = 0xa5a5a5a5;
    datuminstance.two = count;
    success = fifo.insert(datuminstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    printf("%s[%d]: show full\n", __FILE__, __LINE__);

    fifo.show(1);

    printf("%s[%d]: empty\n", __FILE__, __LINE__);

    for (unsigned int jj = 0; count > jj; ++jj) {
        datuminstance.one = 0;
        datuminstance.two = 0;
        success = fifo.peek(datuminstance);
        if (true != success) {
            errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, true, success);
            ++errors;
        }
        if (0xa5a5a5a5 != datuminstance.one) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, 0xa5a5a5a5, datuminstance.one);
            ++errors;
        }
        if ((jj + 1) != datuminstance.two) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, (jj + 1), datuminstance.two);
            ++errors;
        }
        datuminstance.one = 0;
        datuminstance.two = 0;
        success = fifo.remove(datuminstance);
        if (true != success) {
            errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, true, success);
            ++errors;
        }
        if (0xa5a5a5a5 != datuminstance.one) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, 0xa5a5a5a5, datuminstance.one);
            ++errors;
        }
        if ((jj + 1) != datuminstance.two) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, (jj + 1), datuminstance.two);
            ++errors;
        }
    }

    datuminstance.one = 0;
    datuminstance.two = 0;
    success = fifo.remove(datuminstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    printf("%s[%d]: show empty again\n", __FILE__, __LINE__);

    fifo.show(1);

    printf("%s[%d]: fill again\n", __FILE__, __LINE__);

    for (unsigned int jj = 0; count > jj; ++jj) {
        datuminstance.one = 0xa5a5a5a5;
        datuminstance.two = jj + 1;
        success = fifo.insert(datuminstance);
        if (true != success) {
            errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, true, success);
            ++errors;
        }
    }

    datuminstance.one = 0xa5a5a5a5;
    datuminstance.two = count;
    success = fifo.insert(datuminstance);
    if (false != success) {
        errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, false, success);
        ++errors;
    }

    printf("%s[%d]: show full again\n", __FILE__, __LINE__);

    fifo.show(1);

    printf("%s[%d]: reset\n", __FILE__, __LINE__);

    fifo.reset();

    printf("%s[%d]: show empty once again\n", __FILE__, __LINE__);

    fifo.show(1);

    printf("%s[%d]: roll over\n", __FILE__, __LINE__);

    NewFifo<uint16_t> sample(6);
    sample.show(1);

    uint16_t shortinstance0 = 0;
    uint16_t shortinstance1 = 0;
    uint16_t shortinstance2;
    uint16_t shortinstance3 = 0;
    uint16_t shortinstance4;

    for (uint32_t kk = 0; 0x10001UL > kk; ++kk) {
        do {
            success = sample.insert(shortinstance0);
            if (success) {
                ++shortinstance0;
            }
        } while (success);
        do {
            shortinstance2 = ~shortinstance1;
            success = sample.peek(shortinstance2);
            if (success) {
                if (shortinstance1 != shortinstance2) {
                    errorf("%s[%d]: (%u!=%u)!\n",
                        __FILE__, __LINE__, shortinstance1, shortinstance2);
                    ++errors;
                }
                ++shortinstance1;
            }
            shortinstance4 = ~shortinstance3;
            success = sample.remove(shortinstance4);
            if (success) {
                if (shortinstance3 != shortinstance4) {
                    errorf("%s[%d]: (%u!=%u)!\n",
                        __FILE__, __LINE__, shortinstance3, shortinstance4);
                    ++errors;
                }
                ++shortinstance3;
            }
        } while (success);
    }

    sample.show(1);

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    return errors;
}
