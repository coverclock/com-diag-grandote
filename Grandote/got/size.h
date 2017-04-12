/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_SIZE_
#define _H_COM_DIAG_UNITTEST_SIZE_

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
#include "com/diag/desperado/size.h"
#include "com/diag/desperado/BufferOutput.h"
#include "com/diag/desperado/BufferInput.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/DescriptorInput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/Packet.h"
#include "com/diag/desperado/stdlib.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::desperado;

typedef Fixture SizeTest;

TEST_F(SizeTest, Size) {
	Size namesize = size("unittest.txt");
	EXPECT_TRUE(namesize > 0);
	/**/
	int fd = ::open("unittest.txt", O_RDONLY);
	EXPECT_TRUE(fd >= 0);
	Size fdsize = size(fd);
	EXPECT_EQ(fdsize, namesize);
	/**/
	FILE * fp = ::fopen("unittest.txt", "r");
	EXPECT_TRUE(fp != 0);
	Size fpsize = size(fp);
	EXPECT_EQ(fpsize, namesize);
	/**/
	::com::diag::desperado::DescriptorInput descriptorinput(fd);
	Size descriptorsize = size(descriptorinput);
	EXPECT_EQ(descriptorsize, namesize);
	/**/
	::com::diag::desperado::PathInput pathinput("unittest.txt");
	Size pathsize = size(pathinput);
	EXPECT_EQ(pathsize, namesize);
	/**/
	Packet packet;
	Size sourcesize = packet.source(pathinput);
	EXPECT_EQ(sourcesize, namesize);
	Size packetsize = size(packet);
	EXPECT_EQ(packetsize, namesize);
	/**/
	Size packetinputsize = size(packet.input());
	EXPECT_EQ(packetinputsize, namesize);
	/**/
	char * buffer = new char [sourcesize];
	EXPECT_NE(buffer, (char *)0);
	::com::diag::desperado::BufferOutput bufferoutput(buffer, sourcesize);
	Size sinksize = packet.sink(bufferoutput);
	EXPECT_EQ(sinksize, namesize);
	/**/
	::com::diag::desperado::BufferInput bufferinput(bufferoutput.getBuffer(), bufferoutput.getOffset());
	Size buffersize = size(bufferinput);
	EXPECT_EQ(buffersize, namesize);
	/**/
	::com::diag::desperado::DataInput datainput(bufferoutput.getBuffer(), bufferoutput.getOffset());
	Size datasize = size(datainput);
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
	::com::diag::desperado::Input input;
	EXPECT_EQ(size(input), EOF);
}

}
}
}

#endif
