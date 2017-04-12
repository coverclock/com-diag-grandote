/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/MaskableLogger.h"
#include "com/diag/desperado/Mutex.h"
#include "com/diag/desperado/CancellableCriticalSection.h"
#include "com/diag/desperado/LogOutput.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/uint16_Number.h"

namespace com {
namespace diag {
namespace desperado {

static Mutex mutex;

static Mutex serializer;

static MaskableLogger * instant = 0;

MaskableLogger * MaskableLogger::singleton = 0;

MaskableLogger & MaskableLogger::factory() {
    return (*(new MaskableLogger))
    	.disable(FINEST)
    	.disable(FINER)
    	.disable(FINE)
    	.disable(TRACE)
    	.disable(DEBUG)
    	.disable(INFORMATION)
    	.disable(CONFIGURATION)
    	.enable(NOTICE)
    	.enable(WARNING)
    	.enable(ERROR)
    	.enable(SEVERE)
    	.enable(CRITICAL)
    	.enable(ALERT)
    	.enable(FATAL)
    	.enable(EMERGENCY)
    	.enable(PRINT);
}

MaskableLogger & MaskableLogger::instance(MaskableLogger & that) {
	CancellableCriticalSection guard(mutex);
	singleton = &that;
	return *singleton;
}

MaskableLogger & MaskableLogger::instance() {
	CancellableCriticalSection guard(mutex);
	if (singleton == 0) {
		delete instant;
		instant = singleton = &(factory());
	}
    return *singleton;
}

MaskableLogger & MaskableLogger::setMask() {
	const char * name = MASK_ENV();
	const char * string = std::getenv(name);
	if (string != 0) {
		Mask value;
		size_t length;
		if (::com::diag::desperado::uint16_Number(string, value, length)) {
			mask = value;
		}
	}
	return *this;
}

ssize_t MaskableLogger::emit(const char* buffer, size_t size) {
	CancellableCriticalSection guard(serializer);
	ssize_t rc = Logger::emit(buffer, size);
	(output())();
	return rc;
}

void MaskableLogger::show(int level, ::com::diag::desperado::Output * display, int indent) const {
	::com::diag::desperado::Platform& pl = ::com::diag::desperado::Platform::instance();
	::com::diag::desperado::Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    Logger::show(level, display, indent + 1);
    printf("%s mask=0x%x\n", sp, mask);
}

}
}
}
