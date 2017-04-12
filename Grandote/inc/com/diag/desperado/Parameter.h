/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_PARAMETER_
#define _H_COM_DIAG_DESPERADO_PARAMETER_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include <string>
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"

namespace com {
namespace diag {
namespace desperado {
class Input;
}
}
}

namespace com {
namespace diag {
namespace desperado {

/**
 * Parameter implements a general mechanism to map C strings, C++ strings, and
 * Desperado Input functors, into a C string that can be used as a parameter
 * to a method or function.
 */
class Parameter {

private:

	std::string parameter;

public:

	/**
	 * Ctor.
	 *
	 * @param pa points to a C string.
	 */
	explicit Parameter(const char * pa);

	/**
	 * Ctor.
	 *
	 * @param pa refers to a C++ string.
	 */
	explicit Parameter(const std::string & pa);

	/**
	 * Ctor.
	 *
	 * @param input refers to a Desperado Input functor.
	 * @param maximum is the maximum number of characters to consume from the
	 *        functor.
	 */
	explicit Parameter(Input & input, size_t maximum = intmaxof(size_t));

	/**
	 * Ctor.
	 *
	 * @param input points to a Desperado Input functor. The functor is TAKEN
	 *        and deleted after the object is finished with it. (This actually
	 *        occurs in the constructor, not the destructor.)
	 * @param maximum is the maximum number of characters to consume from the
	 *        functor.
	 */
	explicit Parameter(Input * input /* TAKEN */, size_t maximum = intmaxof(size_t));

	/**
	 * Dtor.
	 */
	virtual ~Parameter() {}

	/**
	 * Get the parameter as a C string pointer.
	 *
	 * @return a C string pointer.
	 */
	operator const char *() const { return parameter.c_str(); }

	/**
	 * Get the length of the parameter in bytes.
	 *
	 * @return the length of the parameter.
	 */
	operator int() const { return parameter.length(); }

private:

	void initialize(Input & input, size_t maximum);

};

}
}
}

#endif
