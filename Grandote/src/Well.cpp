/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2013-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txth<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Grandote.html<BR>
 */

extern "C" {
#include "com/diag/diminuto/diminuto_well.h"
#include "com/diag/diminuto/diminuto_criticalsection.h"
}

#include "com/diag/grandote/Well.h"

namespace com {
 namespace diag {
  namespace grandote {

/*******************************************************************************
 * BASE WELL
 ******************************************************************************/

BaseWell::BaseWell(size_t ss, size_t cc, bool mm, size_t aa, size_t pp, size_t ll)
: size(ss)
, cardinality(cc)
, alignment(aa)
, pagesize(pp)
, linesize(ll)
, wellp(static_cast<diminuto_well_t *>(0))
{
	if (cc == 0) {
		/* Do nothing. */
	} else if (!mm) {
		/* Do nothing. */
	} else {
		init(size, cardinality, alignment, pagesize, linesize);
	}
}

BaseWell::~BaseWell() {
	fini();
}

void BaseWell::init(size_t ss, size_t cc, size_t aa, size_t pp, size_t ll) {
	if (wellp == static_cast<diminuto_well_t *>(0)) {
		wellp = diminuto_well_init(ss, cc, aa, pp, ll);
	}
}

void BaseWell::init() {
	init(size, cardinality, alignment);
}

void BaseWell::fini() {
	if (wellp == static_cast<diminuto_well_t *>(0)) {
		/* Do nothing. */
	} else if (diminuto_well_isfull(wellp)) {
		diminuto_well_fini(wellp);
		wellp = (diminuto_well_t *)0;
	} else {
		/* Do nothing. */
	}
}

void * BaseWell::alloc() {
	void * that;
	init();
	that = diminuto_well_alloc(wellp);
	return that;
}

void BaseWell::free(void * pointer) {
	diminuto_well_free(wellp, pointer);
}

bool BaseWell::isFull() const {
	return (diminuto_well_isfull(wellp) != 0);
}

bool BaseWell::isEmpty() const {
	return (diminuto_well_isempty(wellp) != 0);
}

/*******************************************************************************
 * SAFE BASE WELL
 ******************************************************************************/

SafeBaseWell::SafeBaseWell(size_t ss, size_t cc, bool mm, size_t aa, size_t pp, size_t ll)
: BaseWell(ss, cc, mm, aa, pp, ll)
{
	pthread_mutex_init(&mutex, (pthread_mutexattr_t *)0);
}

SafeBaseWell::~SafeBaseWell() {
	pthread_mutex_destroy(&mutex);
}

void SafeBaseWell::init() {
	DIMINUTO_CRITICAL_SECTION_BEGIN(&mutex);
		BaseWell::init();
	DIMINUTO_CRITICAL_SECTION_END;
}

void SafeBaseWell::init(size_t ss, size_t cc, size_t aa, size_t pp, size_t ll) {
	DIMINUTO_CRITICAL_SECTION_BEGIN(&mutex);
		BaseWell::init(ss, cc, aa, pp, ll);
	DIMINUTO_CRITICAL_SECTION_END;
}

void SafeBaseWell::fini() {
	DIMINUTO_CRITICAL_SECTION_BEGIN(&mutex);
		BaseWell::fini();
	DIMINUTO_CRITICAL_SECTION_END;
}

void * SafeBaseWell::alloc() {
	void * that;
	DIMINUTO_CRITICAL_SECTION_BEGIN(&mutex);
		that = BaseWell::alloc();
	DIMINUTO_CRITICAL_SECTION_END;
	return that;
}

void SafeBaseWell::free(void * pointer) {
	DIMINUTO_CRITICAL_SECTION_BEGIN(&mutex);
		BaseWell::free(pointer);
	DIMINUTO_CRITICAL_SECTION_END;
}

  }
 }
}
