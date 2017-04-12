/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "gtest/gtest.h"
#include "Seconds.h"
#include "MaskableLogger.h"
#include "Packet.h"
#include "Parameter.h"
#include "size.h"
#include "Fibonacci.h"
#include "Thread.h"

// Hooks to clean up singletons just for valgrind.

class Platform : public ::com::diag::desperado::Platform {
public:
	static void finalize() { delete singleton; singleton = 0; }
};

class MaskableLogger : public ::com::diag::desperado::MaskableLogger {
public:
	static void finalize() { delete singleton; singleton = 0; }
};

int main(int argc, char ** argv, char **envp) {
    ::testing::InitGoogleTest(&argc, argv);
    int rc = RUN_ALL_TESTS();
 	MaskableLogger::finalize();
	Platform::finalize();
	return rc;
}
