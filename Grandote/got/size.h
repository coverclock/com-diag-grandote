/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_GRANDOTE_UNITTEST_SIZE_
#define _H_COM_DIAG_GRANDOTE_UNITTEST_SIZE_

/**
 * @file
 *
 * Copyright 2011-3013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <sys/stat.h>
#include <fcntl.h>
#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/grandote/size.h"
#include "com/diag/grandote/BufferOutput.h"
#include "com/diag/grandote/BufferInput.h"
#include "com/diag/grandote/DataInput.h"
#include "com/diag/grandote/DescriptorInput.h"
#include "com/diag/grandote/PathInput.h"
#include "com/diag/grandote/Packet.h"
#include "com/diag/grandote/stdlib.h"
#include "com/diag/grandote/stdio.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::grandote;

typedef Fixture SizeTest;

TEST_F(SizeTest, Size) {
	Size namesize = size("dat/unittest.txt");
    fprintf(stderr, "namesize=%zu\n", namesize);
	EXPECT_TRUE(namesize > 0);
	/**/
	int fd = ::open("dat/unittest.txt", O_RDONLY);
	EXPECT_TRUE(fd >= 0);
	Size fdsize = size(fd);
    fprintf(stderr, "fdsize=%zu\n", fdsize);
	EXPECT_EQ(fdsize, namesize);
	/**/
	FILE * fp = ::fopen("dat/unittest.txt", "r");
	ASSERT_TRUE(fp != 0);
	Size fpsize = size(fp);
    fprintf(stderr, "fpsize=%zu\n", fpsize);
	EXPECT_EQ(fpsize, namesize);
	/**/
	DescriptorInput descriptorinput(fd);
	Size descriptorsize = size(descriptorinput);
    fprintf(stderr, "descriptorsize=%zu\n", descriptorsize);
	EXPECT_EQ(descriptorsize, namesize);
	/**/
	PathInput pathinput("dat/unittest.txt");
	Size pathsize = size(pathinput);
    fprintf(stderr, "pathsize=%zu\n", pathsize);
	EXPECT_EQ(pathsize, namesize);
	/**/
	Packet packet;
	Size sourcesize = packet.source(pathinput);
	EXPECT_EQ(sourcesize, namesize);
	Size packetsize = size(packet);
    fprintf(stderr, "packetsize=%zu\n", packetsize);
	EXPECT_EQ(packetsize, namesize);
	/**/
	Size packetinputsize = size(packet.input());
    fprintf(stderr, "packetinputsize=%zu\n", packetinputsize);
	EXPECT_EQ(packetinputsize, namesize);
	/**/
	char * buffer = new char [sourcesize];
	EXPECT_NE(buffer, (char *)0);
	BufferOutput bufferoutput(buffer, sourcesize);
	Size sinksize = packet.sink(bufferoutput);
    fprintf(stderr, "sinksize=%zu\n", sinksize);
	EXPECT_EQ(sinksize, namesize);
	/**/
	BufferInput bufferinput(bufferoutput.getBuffer(), bufferoutput.getOffset());
	Size buffersize = size(bufferinput);
    fprintf(stderr, "buffersize=%zu\n", buffersize);
	EXPECT_EQ(buffersize, namesize);
	/**/
	DataInput datainput(bufferoutput.getBuffer(), bufferoutput.getOffset());
	Size datasize = size(datainput);
    fprintf(stderr, "datasize=%zu\n", datasize);
	EXPECT_EQ(datasize, namesize);
	/**/
	delete [] buffer;
	EXPECT_EQ(::fclose(fp), 0);
	EXPECT_EQ(::close(fd), 0);
	/**/
	EXPECT_EQ(size("./DOES_NOT_EXIST"), EOF);
	EXPECT_EQ(size(-1), EOF);
	EXPECT_EQ(size(STDIN_FILENO), EOF);
	EXPECT_EQ(size(STDOUT_FILENO), EOF);
	EXPECT_EQ(size(STDERR_FILENO), EOF);
	EXPECT_EQ(size((FILE *)0), EOF);
	EXPECT_EQ(size(stdin), EOF);
	EXPECT_EQ(size(stdout), EOF);
	EXPECT_EQ(size(stderr), EOF);
	Input input;
	EXPECT_EQ(size(input), EOF);
}

}
}
}

#endif
