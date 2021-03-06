/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2013-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Grandote.html<BR>
 *
 * With -O3 optimization, run on a
 *
 * vendor_id       : GenuineIntel
 * cpu family      : 6
 * model           : 15
 * model name      : Intel(R) Core(TM)2 Quad CPU    Q6600  @ 2.40GHz
 * cpu MHz         : 1600.000
 * cache size      : 4096 KB
 *
 * this program yields
 *
 * TEST 0: BEGIN
 * TEST 0: END 538965us
 * TEST 1: BEGIN
 * TEST 1: END 417154us
 * TEST 2: BEGIN
 * TEST 2: END 418591us
 * TEST 3: BEGIN
 * TEST 3: END 1303391us
 * TEST 4: BEGIN
 * TEST 4: END 1300946us
 */

extern "C" {
#include "com/diag/diminuto/diminuto_well.h"
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/diminuto/diminuto_countof.h"
#include "com/diag/diminuto/diminuto_time.h"
#include "com/diag/diminuto/diminuto_frequency.h"
}

#include "com/diag/grandote/Well.h"

#include <stdio.h>
#include <stdlib.h>

enum {
	ITERATIONS = 100000,
	CARDINALITY = 1000,
};

class Framistat {

public:

	int skoshification;

	Framistat()
	: skoshification(-1)
	{}

	Framistat(int skosh)
	: skoshification(skosh)
	{}

	virtual ~Framistat() {}

	int discombobulate() {
		return skoshification;
	}

};

class Doohickey : public Framistat {

public:

	Doohickey()
	: Framistat()
	{}

	Doohickey(int skosh)
	: Framistat(skosh)
	{}

	virtual ~Doohickey() {}

	GRANDOTE_WELL_DECLARATION(Doohickey);

};

GRANDOTE_WELL_DEFINITION(Doohickey, CARDINALITY);

class Thingamajig : public Framistat {

public:

	Thingamajig()
	: Framistat()
	{}

	Thingamajig(int skosh)
	: Framistat(skosh)
	{}

	virtual ~Thingamajig() {}

	GRANDOTE_SAFEWELL_DECLARATION(Thingamajig);

};

GRANDOTE_SAFEWELL_DEFINITION(Thingamajig, CARDINALITY);

int main(int argc, char ** argv) {
	Framistat * framistat[CARDINALITY];
	Doohickey * doohickey[countof(framistat)];
	Thingamajig * thingamajig[countof(framistat)];
	size_t ii;
	size_t jj;
	int mask;
	int bit;
	diminuto_ticks_t time;
	diminuto_ticks_t frequency;

	SETLOGMASK();

	mask = (argc < 2) ? ~0 : atoi(argv[1]);

	frequency = diminuto_frequency();

	/* No well. */

	bit = 0;
	if ((mask & (1 << bit)) != 0) {
		DIMINUTO_LOG_DEBUG("TEST %d: BEGIN\n", bit);
		time = diminuto_time_thread();
		for (ii = 0; ii < ITERATIONS; ++ii) {
			for (jj = 0; jj < countof(framistat); ++jj) {
				framistat[jj] = new Framistat(jj);
				ASSERT(framistat[jj] != (Framistat *)0);
			}
			for (jj = 0; jj < countof(framistat); ++jj) {
				delete framistat[jj];
			}
		}
		DIMINUTO_LOG_DEBUG("TEST %d: END %12.9lf seconds\n", bit, (double)(diminuto_time_thread() - time) / frequency);
	}

	/* Thread-unsafe well. */

	bit = 1;
	if ((mask & (1 << bit)) != 0) {
		DIMINUTO_LOG_DEBUG("TEST %d: BEGIN\n", bit);
		time = diminuto_time_thread();
		for (ii = 0; ii < ITERATIONS; ++ii) {
			for (jj = 0; jj < countof(doohickey); ++jj) {
				doohickey[jj] = new Doohickey(jj);
				ASSERT(doohickey[jj] != (Doohickey *)0);
			}
			for (jj = 0; jj < countof(doohickey); ++jj) {
				delete doohickey[jj];
			}
		}
		DIMINUTO_LOG_DEBUG("TEST %d: END %12.9lf seconds\n", bit, (double)(diminuto_time_thread() - time) / frequency);
	}

	/* Thread-unsafe well from base class. */

	bit = 2;
	if ((mask & (1 << bit)) != 0) {
		DIMINUTO_LOG_DEBUG("TEST %d: BEGIN\n", bit);
		time = diminuto_time_thread();
		for (ii = 0; ii < ITERATIONS; ++ii) {
			for (jj = 0; jj < countof(framistat); ++jj) {
				framistat[jj] = new Doohickey(jj);
				ASSERT(framistat[jj] != (Framistat *)0);
			}
			for (jj = 0; jj < countof(framistat); ++jj) {
				delete framistat[jj];
			}
		}
		DIMINUTO_LOG_DEBUG("TEST %d: END %12.9lf seconds\n", bit, (double)(diminuto_time_thread() - time) / frequency);
	}

	/* Thread-safe well. */

	bit = 3;
	if ((mask & (1 << bit)) != 0) {
		DIMINUTO_LOG_DEBUG("TEST %d: BEGIN\n", bit);
		time = diminuto_time_thread();
		for (ii = 0; ii < ITERATIONS; ++ii) {
			for (jj = 0; jj < countof(thingamajig); ++jj) {
				thingamajig[jj] = new Thingamajig(jj);
				ASSERT(thingamajig[jj] != (Thingamajig *)0);
			}
			for (jj = 0; jj < countof(thingamajig); ++jj) {
				delete thingamajig[jj];
			}
		}
		DIMINUTO_LOG_DEBUG("TEST %d: END %12.9lf seconds\n", bit, (double)(diminuto_time_thread() - time) / frequency);
	}

	/* Thread-safe well from base class. */

	bit = 4;
	if ((mask & (1 << bit)) != 0) {
		DIMINUTO_LOG_DEBUG("TEST %d: BEGIN\n", bit);
		time = diminuto_time_thread();
		for (ii = 0; ii < ITERATIONS; ++ii) {
			for (jj = 0; jj < countof(framistat); ++jj) {
				framistat[jj] = new Thingamajig(jj);
				ASSERT(framistat[jj] != (Framistat *)0);
			}
			for (jj = 0; jj < countof(framistat); ++jj) {
				delete framistat[jj];
			}
		}
		DIMINUTO_LOG_DEBUG("TEST %d: END %12.9lf seconds\n", bit, (double)(diminuto_time_thread() - time) / frequency);
	}

	EXIT();
}
