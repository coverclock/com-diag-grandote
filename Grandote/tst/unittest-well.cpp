/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2013-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Grandote.html<BR>
 *
 * It is really helpful to run this with the valgrind tool.
 */

extern "C" {
#include "com/diag/diminuto/diminuto_well.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_countof.h"
}

#include "com/diag/grandote/Well.h"

#include <stdio.h>

enum {
	CARDINALITY = 10,
	LIMIT = 4,
};

class Framistat {

public:

	int skoshification;

	Framistat()
	: skoshification(-1)
	{
		DIMINUTO_LOG_DEBUG("%p->Framistat() skoshification=%d\n", this, skoshification);
	}

	Framistat(int skosh)
	: skoshification(skosh)
	{
		DIMINUTO_LOG_DEBUG("%p->Framistat(%d) skoshification=%d\n", this, skosh, skoshification);
	}

	~Framistat() {
		DIMINUTO_LOG_DEBUG("%p->~Framistat() skoshification=%d\n", this, skoshification);
	}

	int discombobulate() {
		return skoshification;
	}

	GRANDOTE_WELL_DECLARATION(Framistat);

};

GRANDOTE_WELL_DEFINITION(Framistat, CARDINALITY);

int main(int argc, char ** argv) {
	Framistat * framistat[CARDINALITY];
	Framistat * fail;
	Framistat * array;
	size_t ii;
	size_t jj;

	SETLOGMASK();

	DIMINUTO_LOG_DEBUG("sizeof(Framistat)=%zu\n", sizeof(Framistat));

	for (jj = 0; jj < LIMIT; ++jj) {

		ASSERT(Framistat::com_diag_diminuto_well.isFull());
		for (ii = 0; ii < countof(framistat); ++ii) {
			framistat[ii] = new Framistat(ii);
			ASSERT(framistat[ii] != (Framistat *)0);
			ASSERT(!Framistat::com_diag_diminuto_well.isFull());
		}
		ASSERT(Framistat::com_diag_diminuto_well.isEmpty());

		for (ii = 0; ii < countof(framistat); ++ii) {
			ASSERT(framistat[ii]->discombobulate() == ii);
		}

#if !defined(COM_DIAG_DIMINUTO_PLATFORM_BIONIC)
		for (ii = 0; ii < LIMIT; ++ii) {
			try {
				fail = new Framistat(countof(framistat));
				ASSERT(false);
			} catch (std::bad_alloc oom) {
				/* Do nothing. */
			} catch (...) {
				ASSERT(false);
			}
		}
#endif

		for (ii = 0; ii < LIMIT; ++ii) {
			fail = new(std::nothrow) Framistat(countof(framistat));
			ASSERT(fail == (Framistat *)0);
		}

		ASSERT(Framistat::com_diag_diminuto_well.isEmpty());
		for (ii = 0; ii < countof(framistat); ++ii) {
			delete framistat[countof(framistat) - ii - 1];
			ASSERT(!Framistat::com_diag_diminuto_well.isEmpty());
		}
		ASSERT(Framistat::com_diag_diminuto_well.isFull());

	}

	array = new Framistat[CARDINALITY];
	for (ii = 0; ii < CARDINALITY; ++ii) {
		ASSERT(array[ii].discombobulate() == -1);
	}
	delete [] array;

	EXIT();
}
