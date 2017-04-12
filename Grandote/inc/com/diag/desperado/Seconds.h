/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_SECONDS_
#define _H_COM_DIAG_HDESPERADO_SECONDS_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/types.h"
#include "com/diag/desperado/TimeStamp.h"

namespace com {
namespace diag {
namespace desperado {

/**
 * Date and time of day in seconds past the UNIX Epoch.
 */
typedef int64_t Epochalseconds;

/**
 * Seconds implements mechanisms to convert between Epochalseconds and a date
 * and time of day expressed in calendar notation. Seconds uses the Desperado
 * date and time classes and depends on the Desperado Platform Epoch being set
 * correctly. If you are using an operating system based on Linux and GNU,
 * which seems likely, this is already done for you. To use local time, it
 * also depends on the Desperado Platform Time Zone being set correctly. If
 * you are using Linux and GNU, by default Desperado infers this from your
 * underlying system configuration.
 *
 * Remarkably, according to time(2) and POSIX.1 Annex B 2.2.2, the Linux/GNU
 * system clock does not take Leap Seconds into account. This seems wrong, and
 * has seemed wrong since I first visited this issue in 2006 for the Desperado
 * library. To add insult to injury, the Radiocommunications Sector of the
 * International Telecommunications Union (ITU-R) proposes to stop adding
 * Leap Seconds to Atomic Time to get Universal Coordinated Time (UTC) in
 * January 2012, rendering this issue moot. Even worse, Network Time Protocol
 * (NTP) seems to be running about four minutes ahead of the National Institute
 * of Standards and Technology (NIST) time. The band Chicago was right: does
 * anybody really know what time it is? Does anybody really care?
 */
class Seconds {

private:

	static unsigned int dontcare;

protected:

	ticks_t numerator;

	ticks_t denominator;

	TimeStamp timestamp;

public:

	/**
	 * Ctor.
	 */
	explicit Seconds();

	/**
	 * Dtor.
	 */
	virtual ~Seconds();

	/**
	 * Compute the current time in units of seconds since the UNIX Epoch.
	 *
	 * @return the current time in seconds since the UNIX epoch.
	 */
	Epochalseconds now() const;

	/**
	 * Compute the time in units of seconds since the UNIX Epoch for an
	 * arbitary Common Era (CE) calendar date and Universal Coordinated Time
	 * (UTC) clock time. Local time is not supported because, thanks to Daylight
	 * Saving Time (DST), local time is ambiguous.
	 *
	 * @param year is a CE calendar year. This is just the year we all known
	 *        and love, like 2011.
	 * @param month is a CE calendar month of the year. This is one-based:
	 *        January is month 1.
	 * @param day is a CE calendar day of the month. This is one-based: the
	 *        first day of the month is 1.
	 * @param hour is a UTC hour of the day. This is zero-based: the first hour
	 *        of the day is 0.
	 * @param minute is a UTC minute of the hour. This is zero-based: the first
	 *        minute of the hour is 0.
	 * @param second is a UTC second of the minute. This is zero-based: the
	 *        first second of the minute is 0.
	 * @return the date and time in seconds since the UNIX epoch.
	 */
	Epochalseconds then(unsigned int year = 1970, unsigned int month = 1, unsigned int day = 1, unsigned int hour = 0, unsigned int minute = 0, unsigned int second = 0) const;

	/**
	 * Generate a C string that is a ISO8601-ish timestamp for Common Era date
	 * and Universal Coordinated Time for a CE date and UTC time expressed in
	 * seconds since the UNIX Epoch. The individual date and time values are
	 * optionally returned as well.
	 *
	 * @param seconds is seconds since the UNIX Epoch.
	 * @param year refers to a variable into which the year is returned.
	 * @param month refers to a variable into which the month is returned.
	 * @param day refers to a variable into which the day is returned.
	 * @param hour refers to a variable into which the hour is returned.
	 * @param minute refers to a variable into which the minute is returned.
	 * @param second refers to a variable into which the second is returned.
	 * @return a CE/UTC timestamp.
	 */
	const char * zulu(Epochalseconds seconds, unsigned int & year = dontcare, unsigned int & month = dontcare, unsigned int & day = dontcare, unsigned int & hour = dontcare, unsigned int & minute  = dontcare, unsigned int & second = dontcare);

	/**
	 * Generate a C string that is a ISO8601-ish timestamp for Common Era date
	 * and Local Time for a CE date and UTC time expressed in seconds since the
	 * UNIX Epoch. The individual date and time values are optionally returned
	 * as well.
	 *
	 * @param seconds is seconds since the UNIX Epoch.
	 * @param year refers to a variable into which the year is returned.
	 * @param month refers to a variable into which the month is returned.
	 * @param day refers to a variable into which the day is returned.
	 * @param hour refers to a variable into which the hour is returned.
	 * @param minute refers to a variable into which the minute is returned.
	 * @param second refers to a variable into which the second is returned.
	 * @return a CE/Local timestamp.
	 */
	const char * juliet(Epochalseconds seconds, unsigned int & year = dontcare, unsigned int & month = dontcare, unsigned int & day = dontcare, unsigned int & hour = dontcare, unsigned int & minute  = dontcare, unsigned int & second = dontcare);

};

}
}
}

#endif
