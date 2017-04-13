/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _COM_DIAG_GRANDOTE_DIMINUTO_H_
#define _COM_DIAG_GRANDOTE_DIMINUTO_H_

/**
 *  @file
 *
 *  Copyright 2008-2017 Digital Aggregates Corporation, Colorado, USA.
 *
 *  Declares the Diminuto class.
 *
 *  @see    Diminuto
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#if defined(GRANDOTE_PLATFORM_IS_Diminuto)


#include "com/diag/grandote/Linux.h"


#include "com/diag/grandote/Begin.h"

/**
 *  Implements the Platform interface to the Diminuto Linux operating system.
 */
class Diminuto : public Linux {

public:

    /**
     *  Constructor.
     */
    explicit Diminuto();

    /**
     *  Destructor.
     */
    virtual ~Diminuto();

    /**
     *  Returns the frequency of the Linux clock in Hertz as a
     *  a ratio of a numerator and a denominator.
     *
     *  @param  numerator       refers to a variable into which
     *                          the numerator is returned.
     *
     *  @param  denominator     refers to a variable into which
     *                          the denominator is returned.
     */
    virtual void frequency(ticks_t& numerator, ticks_t& denominator);

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
     *                  sent. If null (zero), the default Linux output
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
    virtual void show(int level = 0, Output* display = 0, int indent = 0) const;

private:

    /**
     *  Copy constructor.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Diminuto(const Diminuto& that);

    /**
     *  Assignment operator.
     *
     *  @param  that    refers to an R-value object of this type.
     */
    Diminuto& operator=(const Diminuto& that);

};

#include "com/diag/grandote/End.h"


#endif


#endif
