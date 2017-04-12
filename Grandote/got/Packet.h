/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_PACKET_
#define _H_COM_DIAG_UNITTEST_PACKET_

/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include <string>
#include "gtest/gtest.h"
#include "com/diag/desperado/Packet.h"
#include "com/diag/desperado/size.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/BufferOutput.h"
#include "com/diag/desperado/PathInput.h"
#include "com/diag/desperado/DescriptorOutput.h"
#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/string.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::desperado;

typedef Fixture PacketDataTest;

TEST_F(PacketDataTest, Heap) {
	static const size_t SIZE = 256;
	char data[SIZE];
	PacketData * pdp = new PacketData(data, sizeof(data));
	delete pdp;
}

TEST_F(PacketDataTest, InitialDefault) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, InitialEither) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::EITHER);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, InitialAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::APPEND);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, InitialPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data), PacketData::PREPEND);
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.buffer(), (void *)data);
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char datum = 0;
	EXPECT_EQ(packetdata.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetdata.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, ConsumeOnce) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char buffer[sizeof(data)];
	EXPECT_EQ(packetdata.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), ZERO);
	EXPECT_EQ(packetdata.prefix(), SIZE);
	EXPECT_EQ(packetdata.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, ConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.length(), SIZE);
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetdata.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), ZERO);
	EXPECT_EQ(packetdata.prefix(), SIZE);
	EXPECT_EQ(packetdata.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketDataTest, BufferSize) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	PacketData packetdata(data, sizeof(data));
	EXPECT_FALSE(packetdata.empty());
	EXPECT_EQ(packetdata.size(), SIZE);
	EXPECT_EQ(packetdata.length(), sizeof(data));
	EXPECT_EQ(packetdata.prefix(), ZERO);
	EXPECT_EQ(packetdata.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetdata.buffer(), packetdata.length()), 0);
	packetdata.clear();
	EXPECT_TRUE(packetdata.empty());
	EXPECT_EQ(packetdata.length(), ZERO);
	EXPECT_EQ(packetdata.prefix(), SIZE);
	EXPECT_EQ(packetdata.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetdata.consume(&datum, sizeof(datum)), ZERO);
}

typedef Fixture PacketDataDynamicTest;

TEST_F(PacketDataDynamicTest, Heap) {
	static const size_t SIZE = 256;
	PacketDataDynamic::Datum * data = new PacketDataDynamic::Datum [SIZE];
	PacketDataDynamic * pddp = new PacketDataDynamic(data, sizeof(data));
	delete pddp;
}

typedef Fixture PacketBufferTest;

TEST_F(PacketBufferTest, Heap) {
	static const size_t SIZE = 256;
	char data[SIZE];
	PacketData * pbp = new PacketBuffer(data, SIZE);
	delete pbp;
}

typedef Fixture PacketBufferDynamicTest;

TEST_F(PacketBufferDynamicTest, Heap1) {
	static const size_t SIZE = 256;
	PacketBufferDynamic::Datum * data = new PacketBufferDynamic::Datum[SIZE];
	PacketData * pbdp = new PacketBufferDynamic(data, SIZE);
	delete pbdp;
}

TEST_F(PacketBufferDynamicTest, Heap2) {
	static const size_t SIZE = 256;
	PacketData * pbdp = new PacketBufferDynamic(SIZE);
	delete pbdp;
}

TEST_F(PacketBufferDynamicTest, InitialDefault) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, InitialEither) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, InitialAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, InitialPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum = 0;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, AppendOnceAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, AppendOncePrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, AppendManyAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	size_t size = 0;
	size_t suffix = SIZE;
	for (size_t ii = 0; ii < sizeof(data); ++ii) {
		EXPECT_EQ(packetbuffer.append(&data[ii], sizeof(data[ii])), sizeof(data[ii]));
		EXPECT_FALSE(packetbuffer.empty());
		size += sizeof(data[ii]);
		suffix -= sizeof(data[ii]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), ZERO);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependOnceAppend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::APPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependOncePrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependManyPrepend) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::PREPEND);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	size_t size = 0;
	size_t prefix = SIZE;
	for (size_t ii = sizeof(data); ii > 0; --ii) {
		EXPECT_EQ(packetbuffer.prepend(&data[ii-1], sizeof(data[ii-1])), sizeof(data[ii-1]));
		EXPECT_FALSE(packetbuffer.empty());
		size += sizeof(data[ii-1]);
		prefix -= sizeof(data[ii-1]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), ZERO);
	}
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	EXPECT_EQ(std::memcmp(data, packetbuffer.buffer(), packetbuffer.length()), 0);
	char datum = 0;
	EXPECT_EQ(packetbuffer.append(&datum, sizeof(datum)), ZERO);
	EXPECT_EQ(packetbuffer.prepend(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependAppendConsumeOnce) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	size_t size = 0;
	size_t prefix = SIZE / 2;
	size_t suffix = SIZE / 2;
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packetbuffer.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		EXPECT_FALSE(packetbuffer.empty());
		size += sizeof(data[jj]);
		prefix -= sizeof(data[jj]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packetbuffer.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size += sizeof(data[kk]);
		suffix -= sizeof(data[kk]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	char buffer[sizeof(data)];
	EXPECT_EQ(packetbuffer.consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE, PacketBufferDynamic::EITHER);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	size_t size = 0;
	size_t prefix = SIZE / 2;
	size_t suffix = SIZE / 2;
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packetbuffer.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		EXPECT_FALSE(packetbuffer.empty());
		size += sizeof(data[jj]);
		prefix -= sizeof(data[jj]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packetbuffer.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size += sizeof(data[kk]);
		suffix -= sizeof(data[kk]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
		EXPECT_EQ(packetbuffer.suffix(), suffix);
	}
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packetbuffer.suffix(), ZERO);
		EXPECT_EQ(packetbuffer.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
		size -= sizeof(buffer[ii]);
		prefix += sizeof(buffer[ii]);
		EXPECT_EQ(packetbuffer.length(), size);
		EXPECT_EQ(packetbuffer.prefix(), prefix);
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, AppendOnceClear) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.append(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	packetbuffer.clear();
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

TEST_F(PacketBufferDynamicTest, PrependOnceClear) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	PacketBufferDynamic packetbuffer(SIZE);
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.size(), SIZE);
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	EXPECT_EQ(packetbuffer.prepend(data, sizeof(data)), sizeof(data));
	EXPECT_FALSE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), SIZE);
	EXPECT_EQ(packetbuffer.prefix(), ZERO);
	EXPECT_EQ(packetbuffer.suffix(), ZERO);
	packetbuffer.clear();
	EXPECT_TRUE(packetbuffer.empty());
	EXPECT_EQ(packetbuffer.length(), ZERO);
	EXPECT_EQ(packetbuffer.prefix(), SIZE);
	EXPECT_EQ(packetbuffer.suffix(), SIZE);
	char datum;
	EXPECT_EQ(packetbuffer.consume(&datum, sizeof(datum)), ZERO);
}

typedef Fixture PacketTest;

TEST_F(PacketTest, HeapAndShow) {
	Packet * packet = new Packet;
	ASSERT_NE(packet, (Packet*)0);
	packet->show(0, &errput);
	delete packet;
}

TEST_F(PacketTest, EitherPrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::EITHER);
	packet.show(2, &errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	packet.show(2, &errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &errput);
}

TEST_F(PacketTest, PrependPrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::PREPEND);
	packet.show(2, &errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	packet.show(2, &errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &errput);
}

TEST_F(PacketTest, AppendPrependAppendConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::APPEND);
	packet.show(2, &errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
	}
	packet.show(2, &errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &errput);
}

TEST_F(PacketTest, EitherAppendPrependConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::EITHER);
	packet.show(2, &errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
	}
	packet.show(2, &errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &errput);
}

TEST_F(PacketTest, PrependAppendPrependConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::PREPEND);
	packet.show(2, &errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
	}
	packet.show(2, &errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &errput);
}

TEST_F(PacketTest, AppendAppendPrependConsumeMany) {
	static const size_t SIZE = 256;
	static const size_t ZERO = 0;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = SIZE / 8;
	Packet packet(ALLOC, Packet::APPEND);
	packet.show(2, &errput);
	for (size_t ii = 0; ii < (sizeof(data) / 2); ++ii) {
		size_t kk = (sizeof(data) / 2) + ii;
		EXPECT_EQ(packet.append(&data[kk], sizeof(data[kk])), sizeof(data[kk]));
		size_t jj = (sizeof(data) / 2) - ii - 1;
		EXPECT_EQ(packet.prepend(&data[jj], sizeof(data[jj])), sizeof(data[jj]));
	}
	packet.show(2, &errput);
	char buffer[sizeof(data)];
	for (size_t ii = 0; ii < sizeof(buffer); ++ii) {
		EXPECT_EQ(packet.consume(&buffer[ii], sizeof(buffer[ii])), sizeof(buffer[ii]));
	}
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
	char datum;
	EXPECT_EQ(packet.consume(&datum, sizeof(datum)), ZERO);
	packet.show(2, &errput);
}

TEST_F(PacketTest, MixedBag) {
	Packet * packet = new Packet;
	PacketDataDynamic::Datum * data2 = new PacketDataDynamic::Datum [3];
	for (int ii = 0; ii < 3; ++ii) { data2[ii] = 'j' + ii; }
	PacketDataDynamic * pdd = new PacketDataDynamic(data2, 3);
	packet->append(*pdd);
	struct Data1 { char data[7]; } data1 = { { 'c', 'd', 'e', 'F', 'g', 'h', 'i' } };
	PacketData * pd = new PacketData(&data1, sizeof(data1));
	packet->prepend(*pd);
	struct Data3 { char data[11]; } data3;
	PacketBuffer * pb = new PacketBuffer(&data3, sizeof(data3), PacketBuffer::APPEND);
	packet->append(*pb);
	PacketBufferDynamic::Datum * data4 = new PacketBufferDynamic::Datum [2];
	PacketBufferDynamic * pbd1 = new PacketBufferDynamic(data4, 2, PacketBufferDynamic::PREPEND);
	packet->prepend(*pbd1);
	EXPECT_EQ(packet->prepend("ab", 2), (size_t)2);
	PacketBufferDynamic * pbd2 = new PacketBufferDynamic(5, PacketBufferDynamic::APPEND);
	packet->append(*pbd2);
	EXPECT_EQ(packet->append("xyz!", 5), (size_t)5);
	data1.data[3] = 'f'; // Modification!
	EXPECT_EQ(pb->append("mnopqrstuvw", 11), (size_t)11); // Insertion!
	char buffer[28];
	EXPECT_EQ(packet->consume(buffer, sizeof(buffer)), sizeof(buffer));
	EXPECT_EQ(std::strncmp(buffer, "abcdefghijklmnopqrstuvwxyz!", sizeof(buffer)), 0);
	delete packet;
}

typedef Fixture PacketInputOutputTest;

TEST_F(PacketInputOutputTest, Block) {
	static const size_t SIZE = 5 * 11 * 4;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	static const size_t OUTPUT = 5;
	for (size_t ii = 0; ii < sizeof(data); ii += OUTPUT) {
		EXPECT_EQ((packet.output())(&(data[ii]), 1, OUTPUT), OUTPUT);
		EXPECT_FALSE(packet.empty());
	}
	char buffer[sizeof(data)];
	static const size_t INPUT = 11;
	for (size_t ii = 0; ii < sizeof(buffer); ii += INPUT) {
		EXPECT_EQ((packet.input())(&(buffer[ii]), 1, INPUT), INPUT);
	}
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
}

TEST_F(PacketInputOutputTest, Character) {
	static const size_t SIZE = 5 * 11 * 4;
	char data[SIZE];
	for (size_t ii = 0; ii < sizeof(data); ++ii) { data[ii] = ii; }
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	for (size_t ii = 0; ii < sizeof(data); ++ii) {
		EXPECT_EQ((packet.output())((int)data[ii]), (int)data[ii]);
		EXPECT_FALSE(packet.empty());
	}
	char buffer[sizeof(data)];
	int ch;
	int ch2;
	for (size_t ii = 0; ii < sizeof(buffer);++ii) {
		EXPECT_FALSE(packet.empty());
		ch = (packet.input())();
		EXPECT_NE(ch, EOF);
		ch2 = (packet.input())(ch);
		EXPECT_EQ(ch, ch2);
		EXPECT_FALSE(packet.empty());
		ch = (packet.input())();
		EXPECT_NE(ch, EOF);
		EXPECT_EQ(ch, ch2);
		buffer[ii] = ch;
	}
	ch = (packet.input())();
	EXPECT_EQ(ch, EOF);
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(std::memcmp(data, buffer, sizeof(data)), 0);
}

static const char * HENRYV[] = {
	"This day is call'd the feast of Crispian.\n",
	"He that outlives this day, and comes safe home,\n",
	"Will stand a tip-toe when this day is named,\n",
	"And rouse him at the name of Crispian.\n",
	"He that shall live this day, and see old age,\n",
	"Will yearly on the vigil feast his neighbours,\n",
	"And say, \"To-morrow is Saint Crispian.\"\n",
	"Then will he strip his sleeve and show his scars,\n",
	"And say, \"These wounds I had on Crispin's day.\"\n",
	"Old men forget; yet all shall be forgot,\n",
	"But he'll remember with advantages\n",
	"What feats he did that day. Then shall our names,\n",
	"Familiar in his mouth as household words,\n",
	"Harry the King, Bedford, and Exeter,\n",
	"Warwick and Talbot, Salisbury and Gloucester,\n",
	"Be in their flowing cups freshly rememb'red.\n",
	"This story shall the good man teach his son;\n",
	"And Crispin Crispian shall ne'er go by,\n",
	"From this day to the ending of the world,\n",
	"But we in it shall be remembered-\n",
	"We few, we happy few, we band of brothers;\n",
	"For he to-day that sheds his blood with me\n",
	"Shall be my brother; be he ne'er so vile,\n",
	"This day shall gentle his condition:\n",
	"And gentlemen in England now a-bed\n",
	"Shall think themselves accursed they were not here,\n",
	"And hold their manhoods cheap whiles any speaks\n",
	"That fought with us upon Saint Crispin's day.\n",
	"-- William Shakespeare, HENRY V, act IV scene iii\n"
};

TEST_F(PacketInputOutputTest, String) {
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	size_t longest = 0;
	for (size_t ii = 0; ii < countof(HENRYV); ++ii) {
		size_t length = std::strlen(HENRYV[ii]);
		if (length > longest) { longest = length; }
		EXPECT_LE((packet.output())(HENRYV[ii]), length);
		EXPECT_FALSE(packet.empty());
	}
	char buffer[countof(HENRYV)][64];
	ASSERT_LT(longest, 64);
	for (size_t ii = 0; ii < countof(buffer); ++ii) {
		(packet.input())(buffer[ii], sizeof(buffer[ii]));
	}
	EXPECT_TRUE(packet.empty());
	for (size_t ii = 0; ii < countof(buffer); ++ii) {
		EXPECT_EQ(std::strncmp(HENRYV[ii], buffer[ii], sizeof(buffer[ii])), 0);
	}
}

TEST_F(PacketInputOutputTest, Formatted) {
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	::com::diag::desperado::Print print(packet.output());
	for (size_t ii = 0; ii < countof(HENRYV); ++ii) {
		print("%s", HENRYV[ii]);
		EXPECT_FALSE(packet.empty());
	}
	char buffer[countof(HENRYV)][64];
	for (size_t ii = 0; ii < countof(buffer); ++ii) {
		(packet.input())(buffer[ii], sizeof(buffer[ii]));
	}
	EXPECT_TRUE(packet.empty());
	for (size_t ii = 0; ii < countof(buffer); ++ii) {
		EXPECT_EQ(std::strncmp(HENRYV[ii], buffer[ii], sizeof(buffer[ii])), 0);
	}
}

static const char RICHARDII[] = {
	"This royal throne of kings, this sceptred isle,\n"
	"This earth of majesty, this seat of Mars,\n"
	"This other Eden, demi-paradise,\n"
	"This fortress built by Nature for herself\n"
	"Against infection and the hand of war,\n"
	"This happy breed of men, this little world,\n"
	"This precious stone set in the silver sea,\n"
	"Which serves it in the office of a wall\n"
	"Or as a moat defensive to a house,\n"
	"Against the envy of less happier lands,--\n"
	"This blessed plot, this earth, this realm, this England.\n"
	"-- William Shakespeare, KING RICHARD II, act II scene i\n"
};

TEST_F(PacketInputOutputTest, SourceSinkBuffer) {
	::com::diag::desperado::DataInput datainput(RICHARDII, sizeof(RICHARDII));
	static const size_t ALLOC = 7;
	Packet packet(ALLOC, Packet::APPEND);
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(packet.source(datainput), sizeof(RICHARDII));
	EXPECT_FALSE(packet.empty());
	char buffer[sizeof(RICHARDII)];
	::com::diag::desperado::BufferOutput bufferoutput(buffer, sizeof(buffer));
	EXPECT_EQ(packet.sink(bufferoutput), sizeof(RICHARDII));
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(std::strncmp(RICHARDII, buffer, sizeof(buffer)), 0);
}

TEST_F(PacketInputOutputTest, SourceSinkPathFile) {

	::com::diag::desperado::PathInput input("unittest.txt", "r");
	Size inputsize = size(input);
	EXPECT_TRUE(inputsize > 0);
	char name[] = "/tmp/PacketTest.SourceSinkPathFile.XXXXXX";
	int fd = ::mkstemp(name);
	ASSERT_TRUE(fd > 0);
	::com::diag::desperado::DescriptorOutput output(fd);
	Packet packet;
	EXPECT_TRUE(packet.empty());
	size_t sourced = packet.source(input);
	EXPECT_FALSE(packet.empty());
	EXPECT_EQ(sourced, inputsize);
	EXPECT_EQ(packet.length(), inputsize);
	size_t sunk = packet.sink(output);
	EXPECT_TRUE(packet.empty());
	EXPECT_EQ(sunk, inputsize);
	Size outputsize = size(output);
	EXPECT_EQ(outputsize, inputsize);
	EXPECT_EQ(::close(fd), 0);
	std::string command = "diff ";
	command += "unittest.txt";
	command += " ";
	command += name;
	EXPECT_EQ(std::system(command.c_str()), 0);
	EXPECT_EQ(::unlink(name), 0);
}

}
}
}

#endif
