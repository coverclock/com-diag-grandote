/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/Parameter.h"
#include "com/diag/desperado/Input.h"

namespace com {
namespace diag {
namespace desperado {

Parameter::Parameter(const char * pa)
: parameter(pa)
{}

Parameter::Parameter(const std::string & pa)
: parameter(pa)
{}

Parameter::Parameter(Input & input, size_t maximum) {
	initialize(input, maximum);
}

Parameter::Parameter(Input * input /* TAKEN */, size_t maximum) {
	if (input != 0) {
		initialize(*input, maximum);
		delete input;
	}
}

void Parameter::initialize(Input & input, size_t maximum) {
	int ch;
	while (maximum > 0) {
		ch = input();
		if ((ch == EOF) || (ch == '\0') || (ch == '\n')) { break; }
		parameter += ch;
		--maximum;
	}
}

}
}
}
