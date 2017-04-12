/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_MASKABLELOGGER_
#define _H_COM_DIAG_DESPERADO_MASKABLELOGGER_

/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Logger.h"
#include "com/diag/desperado/Platform.h"

namespace com {
namespace diag {
namespace desperado {

/**
 * MaskableLogger extends the Desperado Logger class to implement a mechanism to
 * enable and disable log levels selectively. The superclass supports sixteen
 * logging levels (which turns out to be the transitive closure of logging
 * levels support by common logging systems such as Syslog, Java, and Apache).
 */
class MaskableLogger : public Logger {

public:

    /**
     * This mask holds a bit for every possible log level encoded as (1<<level).
     */
	typedef uint16_t Mask;

protected:

    /**
     * Points to the default instance of this object.
     */
    static MaskableLogger * singleton;

    /**
     * Encodes the log mask where each bit indicates whether the log level
     * is enabled or not, encoded as (1 << level).
     */
    Mask mask;

public:

	/**
	 * Allocates a reference to a new object of this type suitably initialized
	 * with default parameters.
	 *
	 * @return a reference to a new object of this type.
	 */
	static MaskableLogger & factory();

	/**
	 * Establishes a new default instance of an object of this type.
	 *
	 * @param that refers to an object of this type.
	 * @return that.
	 */
	static MaskableLogger & instance(MaskableLogger & that);

    /**
     * Returns a reference to the default instance of an object of this type.
     *
     * @return a reference to the default instance of an object of this type.
     */
	static MaskableLogger & instance();

    /**
     * Returns the name of the environmental variable that can be used to
     * set the mask.
     *
     * @return the name of the environmental variable.
     */
    static const char * MASK_ENV() {
    	return "COM_DIAG_DESPERADO_LOGGER_MASK";
    }

	/**
	 * Ctor.
	 */
    explicit MaskableLogger()
    : Logger(Platform::instance().log())
    , mask(0)
    {
    }

	/**
	 * Ctor.
	 *
	 * @param ro refers to an Output functor to which log messages are emitted.
	 */
    explicit MaskableLogger(Output & ro)
    : Logger(ro)
    , mask(0)
    {
    }

    /**
     * Dtor. Any buffered output is flushed.
     */
    virtual ~MaskableLogger() {
    	(output())();
    }

    /**
     * Set the Output functor to which log messages are emitted.
     *
	 * @param ro refers to an Output functor to which log messages are emitted.
     * @return a reference to this object.
     */
    Logger & setOutput(Output & ro) {
    	Logger logger(ro);
    	//
    	// Yes, I know this is kinda funky. We just want to reinitialize the
    	// super class portion of ourselves with the new version to set the
    	// output functor, and the super class doesn't have an operation with
    	// which to do that, and adding one violates the design of Desperado.
    	//
    	// IMO this is legal and should compile, but does not. Worse, the
    	// diagnostics are completely unhelpful.
    	//  static_cast<::com::diag::desperado::Logger&>(*this) = logger;
    	//
    	// Somewhat to my surprise, this compiles and works, invoking the
    	// default assignment operator in the base class to do the copy.
    	//  ::com::diag::desperado::Logger::operator=(logger);
    	//
    	// But this is a little less scary looking, and works fine.
    	//
    	Logger * that = this;
    	*that = logger;
    	return *this;
    }

    /**
     * Get a reference to the current output functor.
     *
     * @return the current output functor.
     */
    Output & getOutput() {
    	return output();
    }

    /**
     * Set the mask which controls which log levels are enabled.
     *
     * @param vm is the new mask value.
     * @return a reference to this object.
     */
    MaskableLogger & setMask(Mask vm) {
    	mask = vm;
    	return *this;
    }

    /**
     * Set the mask which controls which log levels are enabled from
     * an environmental variable.
     *
     * @return a reference to this object.
     */
    MaskableLogger & setMask();

    /**
     * Get the current mask. This is mostly useful for saving the current mask
     * so that it can be restored after temporarily changing it.
     *
     * @return the current mask;
     */
    Mask getMask() {
    	return mask;
    }

    /**
     * Enable the specified log level.
     *
     * @param level is the log level to be enabled.
     * @return a reference to this object.
     */
    MaskableLogger & enable(Level level) {
    	mask |= ((Mask)1 << level);
    	return *this;
    }

    /**
     * Disable the specified log level.
     *
     * @param level is the log level to be disabled.
     * @return a reference to this object.
     */
    MaskableLogger & disable(Level level) {
    	mask &= ~((Mask)1 << level);
    	return *this;
    }

    /**
     * Returns true if the specified log level is enabled, false otherwise.
     *
     * @return true if the specified log level is enabled, false otherwise.
     */
    virtual bool isEnabled(Level level) {
    	return (mask & ((Mask)1 << level)) != 0;
    }

    /**
     *  Unconditionally emit a log message using a buffer.
     *
     *  @param  buffer  points to the buffer to be emitted.
     *
     *  @param  size    is the size of the buffer in bytes.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    virtual ssize_t emit(const char* buffer, size_t size);

    /**
     *  Displays internal information about this object to the specified
     *  output object. Useful for debugging and troubleshooting.
     *
     *  @param  level   sets the verbosity of the output. What this means
     *                  is object dependent. However, the level is passed
     *                  from outer to inner objects this object calls the
     *                  show methods of its inherited or composited objects.
     *
     *  @param display  points to the output object to which output is
     *                  sent. If null (zero), the default platform output
     *                  object is used as the effective output object. The
     *                  effective output object is passed from outer to
     *                  inner objects as this object calls the show methods
     *                  of its inherited and composited objects.
     *
     *  @param  indent  specifies the level of indentation. One more than
     *                  this value is passed from outer to inner objects
     *                  as this object calls the show methods of its
     *                  inherited and composited objects.
     */
    virtual void show(int level = 0, Output * display = 0, int indent = 0) const;

};

}
}
}

#endif
