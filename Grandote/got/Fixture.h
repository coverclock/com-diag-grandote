/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_FIXTURE_
#define _H_COM_DIAG_UNITTEST_FIXTURE_

/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/MaskableLogger.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::desperado;

class Test : public ::testing::Test {

protected:

	::com::diag::desperado::FileOutput errput;
	::com::diag::desperado::LogOutput logput;
	::com::diag::desperado::Output * output;
	MaskableLogger::Mask mask;

public:

	MaskableLogger & logger;
	::com::diag::desperado::Platform & platform;
	::com::diag::desperado::Print printf;
	::com::diag::desperado::Dump dump;

	explicit Test()
	: errput(::stderr)
	, logput(errput)
	, output(0)
	, mask(0)
	, logger(MaskableLogger::instance())
	, platform(::com::diag::desperado::Platform::instance())
	, printf(errput)
	, dump(errput)
	{}

	virtual MaskableLogger::Mask terse() {
		MaskableLogger::Mask save = logger.getMask();
		logger
			.disable(MaskableLogger::FINEST)
			.disable(MaskableLogger::FINER)
			.disable(MaskableLogger::FINE)
			.disable(MaskableLogger::TRACE)
			.disable(MaskableLogger::DEBUG)
			.disable(MaskableLogger::INFORMATION)
			.disable(MaskableLogger::CONFIGURATION)
			.enable(MaskableLogger::NOTICE)
			.enable(MaskableLogger::WARNING)
			.enable(MaskableLogger::ERROR)
			.enable(MaskableLogger::SEVERE)
			.enable(MaskableLogger::CRITICAL)
			.enable(MaskableLogger::ALERT)
			.enable(MaskableLogger::FATAL)
			.enable(MaskableLogger::EMERGENCY)
			.enable(MaskableLogger::PRINT);
		return save;
	}

	virtual MaskableLogger::Mask verbose() {
		MaskableLogger::Mask save = logger.getMask();
		logger
	    	.enable(MaskableLogger::FINEST)
	    	.enable(MaskableLogger::FINER)
	    	.enable(MaskableLogger::FINE)
	    	.enable(MaskableLogger::TRACE)
	    	.enable(MaskableLogger::DEBUG)
	    	.enable(MaskableLogger::INFORMATION)
	    	.enable(MaskableLogger::CONFIGURATION)
	    	.enable(MaskableLogger::NOTICE)
	    	.enable(MaskableLogger::WARNING)
	    	.enable(MaskableLogger::ERROR)
	    	.enable(MaskableLogger::SEVERE)
	    	.enable(MaskableLogger::CRITICAL)
	    	.enable(MaskableLogger::ALERT)
	    	.enable(MaskableLogger::FATAL)
	    	.enable(MaskableLogger::EMERGENCY)
	    	.enable(MaskableLogger::PRINT);
		return save;
	}

	virtual void restore(MaskableLogger::Mask save) {
		logger.setMask(save);
	}

};

class Terse : public Test {

protected:

	virtual void SetUp() {
		output = &(logger.getOutput());
		logger.setOutput(logput);
		mask = terse();
		logger.setMask();
	}

	virtual void TearDown() {
		restore(mask);
		logger.setOutput(*output);
	}

};

class Verbose : public Test {

protected:

	virtual void SetUp() {
		output = &(logger.getOutput());
		logger.setOutput(logput);
		mask = verbose();
		logger.setMask();
	}

	virtual void TearDown() {
		restore(mask);
		logger.setOutput(*output);
	}

};

/**
 * This type is the default logging strategy. Choose Verbose or Terse. You
 * can always choose the other on specific test cases, or even create your
 * own by deriving your own class.
*/
#if 1
typedef Terse Fixture;
#else
typedef Verbose Fixture;
#endif

}
}
}

#endif
