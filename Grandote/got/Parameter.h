/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_PARAMETER_
#define _H_COM_DIAG_UNITTEST_PARAMETER_

/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/desperado/Parameter.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/DataInput.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::desperado;

typedef Fixture ParameterTest;

TEST_F(ParameterTest, Array) {
	static const char INPUT[] = "/usr/local/etc/array.conf";
	Parameter parameter(INPUT);
	EXPECT_EQ(std::strcmp(parameter, INPUT), 0);
	EXPECT_EQ((int)parameter, std::strlen(INPUT));
}

TEST_F(ParameterTest, String) {
	static const std::string INPUT = "/usr/local/etc/string.conf";
	Parameter parameter(INPUT);
	EXPECT_EQ(std::strcmp(parameter, INPUT.c_str()), 0);
	EXPECT_EQ((int)parameter, std::strlen(INPUT.c_str()));
}

TEST_F(ParameterTest, Stack) {
	static const char INPUT[] = "/usr/local/etc/stack.conf";
	::com::diag::desperado::DataInput input(INPUT);
	Parameter parameter(input);
	EXPECT_EQ(std::strcmp(parameter, INPUT), 0);
	EXPECT_EQ((int)parameter, std::strlen(INPUT));
}

TEST_F(ParameterTest, StackLength) {
	static const char INPUT[] = "/usr/local/etc/stack.conf";
	::com::diag::desperado::DataInput input(INPUT);
	Parameter parameter(input, std::strlen(INPUT) - 5);
	EXPECT_EQ(std::strcmp(parameter, "/usr/local/etc/stack"), 0);
	EXPECT_EQ((int)parameter, std::strlen("/usr/local/etc/stack"));
}

TEST_F(ParameterTest, Heap) {
	static const char INPUT[] = "/usr/local/etc/heap.conf";
	::com::diag::desperado::DataInput * input = new ::com::diag::desperado::DataInput(INPUT);
	Parameter * parameter = new Parameter(input);
	EXPECT_EQ(std::strcmp(*parameter, INPUT), 0);
	EXPECT_EQ((int)*parameter, std::strlen(INPUT));
	delete parameter;
}

static const char PARAMETERTESTINPUT[] = "/usr/local/etc/default.conf";
static const char * parameterfunction(const Parameter & parameter = Parameter(new ::com::diag::desperado::DataInput(PARAMETERTESTINPUT))) {
	return parameter;
}

TEST_F(ParameterTest, DefaultFunctionArgument) {
	EXPECT_EQ(std::strcmp(parameterfunction(), PARAMETERTESTINPUT), 0);
}

}
}
}

#endif
