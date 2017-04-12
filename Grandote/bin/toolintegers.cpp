/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Desperadito library.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

******************************************************************************/


/**
 *  @file
 *
 *  Does a simple eyeball test of integers.h.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/integers.h"
#include "com/diag/desperado/Desperado.h"

int main() {

	printf("uint32_t\n");
	printf("%u\n",  widthOf<uint32_t>());
	printf("0x%8.8x %d\n", signedintminOf<uint32_t>(), signedintminOf<uint32_t>());
	printf("0x%8.8x %d\n", signedintmaxOf<uint32_t>(), signedintmaxOf<uint32_t>());
	printf("0x%8.8x %u\n", unsignedintminOf<uint32_t>(), unsignedintminOf<uint32_t>());
	printf("0x%8.8x %u\n", unsignedintmaxOf<uint32_t>(), unsignedintmaxOf<uint32_t>());
	printf("0x%8.8x %u\n", intminOf<uint32_t>(), intminOf<uint32_t>());
	printf("0x%8.8x %u\n", intmaxOf<uint32_t>(), intmaxOf<uint32_t>());

	printf("int32_t\n");
	printf("%u\n",  widthOf<int32_t>());
	printf("0x%8.8x %d\n", signedintminOf<int32_t>(), signedintminOf<int32_t>());
	printf("0x%8.8x %d\n", signedintmaxOf<int32_t>(), signedintmaxOf<int32_t>());
	printf("0x%8.8x %u\n", unsignedintminOf<int32_t>(), unsignedintminOf<int32_t>());
	printf("0x%8.8x %u\n", unsignedintmaxOf<int32_t>(), unsignedintmaxOf<int32_t>());
	printf("0x%8.8x %d\n", intminOf<int32_t>(), intminOf<int32_t>());
	printf("0x%8.8x %d\n", intmaxOf<int32_t>(), intmaxOf<int32_t>());

	printf("uint32\n");
	uint32_t uint32 = 0;
	printf("%u\n",  widthOf(uint32));
	printf("0x%8.8x %d\n", signedintminOf(uint32), signedintminOf(uint32));
	printf("0x%8.8x %d\n", signedintmaxOf(uint32), signedintmaxOf(uint32));
	printf("0x%8.8x %u\n", unsignedintminOf(uint32), unsignedintminOf(uint32));
	printf("0x%8.8x %u\n", unsignedintmaxOf(uint32), unsignedintmaxOf(uint32));
	printf("0x%8.8x %u\n", intminOf(uint32), intminOf(uint32));
	printf("0x%8.8x %u\n", intmaxOf(uint32), intmaxOf(uint32));

	printf("int32\n");
	int32_t int32 = 0;
	printf("%u\n",  widthOf(int32));
	printf("0x%8.8x %d\n", signedintminOf(int32), signedintminOf(int32));
	printf("0x%8.8x %d\n", signedintmaxOf(int32), signedintmaxOf(int32));
	printf("0x%8.8x %u\n", unsignedintminOf(int32), unsignedintminOf(int32));
	printf("0x%8.8x %u\n", unsignedintmaxOf(int32), unsignedintmaxOf(int32));
	printf("0x%8.8x %d\n", intminOf(int32), intminOf(int32));
	printf("0x%8.8x %d\n", intmaxOf(int32), intmaxOf(int32));

}
