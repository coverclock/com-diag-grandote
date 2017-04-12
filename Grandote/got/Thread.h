/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_THREAD_
#define _H__COM_DIAG_UNITTEST_THREAD_

/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/desperado/Thread.h"
#include "com/diag/desperado/Mutex.h"
#include "com/diag/desperado/CriticalSection.h"
#include "com/diag/desperado/Uncancellable.h"
#include "com/diag/desperado/Condition.h"
#include "com/diag/desperado/MemoryBarrier.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include <pthread.h>

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::desperado;

typedef Fixture ThreadTest;

struct MyMutex : public Mutex {
	int nesting;
	int status;
	explicit MyMutex()
	: nesting(0)
	{}
	virtual int begin() {
		MaskableLogger::instance().debug("MyMutex[0x%lx]::begin\n", Thread::self());
		status = Mutex::begin();
		MaskableLogger::instance().debug("MyMutex[0x%lx]::begin %d %s\n", Thread::self(), nesting, ::strerror(status));
		++nesting;
		return status;
	}
	virtual int end() {
		--nesting;
		MaskableLogger::instance().debug("MyMutex[0x%lx]::end %d\n", Thread::self(), nesting);
		status = Mutex::end();
		MaskableLogger::instance().debug("MyMutex[0x%lx]::end %s\n", Thread::self(), ::strerror(status));
		return status;
	}
};

struct MyCriticalSection : public CriticalSection {
	explicit MyCriticalSection(Mutex & mutexr, bool disable = true)
	: CriticalSection(mutexr, disable)
	{}
	operator int() { return status; }
};

static MyMutex myMutex;

TEST_F(ThreadTest, Mutex) {
	EXPECT_EQ(myMutex.begin(), 0);
	EXPECT_EQ(myMutex.end(), 0);
}

TEST_F(ThreadTest, MutexRecursive) {
	EXPECT_EQ(myMutex.begin(), 0);
	EXPECT_EQ(myMutex.begin(), 0);
	EXPECT_EQ(myMutex.begin(), 0);
	EXPECT_EQ(myMutex.end(), 0);
	EXPECT_EQ(myMutex.end(), 0);
	EXPECT_EQ(myMutex.end(), 0);
}

TEST_F(ThreadTest, CriticalSection) {
	MyCriticalSection guard(myMutex);
	EXPECT_EQ((int)guard, 0);
	EXPECT_EQ(myMutex.status, 0);
}

TEST_F(ThreadTest, CriticalSectionRecursive) {
	{
		MyCriticalSection guard1(myMutex);
		EXPECT_EQ((int)guard1, 0);
		EXPECT_EQ(myMutex.status, 0);
		{
			MyCriticalSection guard2(myMutex);
			EXPECT_EQ((int)guard2, 0);
			EXPECT_EQ(myMutex.status, 0);
			{
				MyCriticalSection guard3(myMutex);
				EXPECT_EQ((int)guard3, 0);
				EXPECT_EQ(myMutex.status, 0);
			}
			EXPECT_EQ(myMutex.status, 0);
		}
		EXPECT_EQ(myMutex.status, 0);
	}
	EXPECT_EQ(myMutex.status, 0);
}

struct ThreadJoin : public Thread {
	int & variable;
	explicit ThreadJoin(int & shared)
	: variable(shared)
	{
		variable = 1;
	}
	virtual void * run() {
		MyCriticalSection guard(myMutex);
		variable = 3;
		return 0;
	}
};

TEST_F(ThreadTest, Idle) {
	int variable = 0;
	ThreadJoin thread(variable);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.notify(), 0);
	EXPECT_TRUE(thread.notified());
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 1);
}

TEST_F(ThreadTest, Join) {
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadJoin thread(variable);
	EXPECT_EQ(variable, 1);
	{
		MyCriticalSection guard(myMutex);
		variable = 2;
		EXPECT_EQ(variable, 2);
		EXPECT_EQ(thread.start(), 0);
		EXPECT_NE(thread.start(), 0);
		EXPECT_EQ(variable, 2);
		platform.yield(platform.frequency());
		EXPECT_EQ(variable, 2);
	}
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 4);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
}

struct ThreadCancel : public Thread {
	int & variable;
	explicit ThreadCancel(int & shared)
	: variable(shared)
	{
		variable = 1;
	}
	virtual void * run() {
		while (true) {
			variable = 3;
			cancellable();
			yield();
		}
		return 0;
	}
};

TEST_F(ThreadTest, Cancel) {
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadCancel thread(variable);
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(variable, 1);
	variable = 2;
	EXPECT_EQ(variable, 2);
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 3);
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_TRUE(thread.cancelled());
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
}

struct ThreadNotify : public Thread {
	int & variable;
	explicit ThreadNotify(int & shared)
	: variable(shared) {
		variable = 1;
	}
	virtual void * run() {
		MyCriticalSection guard(myMutex);
		while (true) {
			variable = 3;
			if (notified()) {
				break;
			}
			yield();
		}
		return 0;
	}
};

TEST_F(ThreadTest, Notify) {
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadNotify thread(variable);
	EXPECT_FALSE(thread.notified());
	EXPECT_EQ(variable, 1);
	variable = 2;
	EXPECT_EQ(variable, 2);
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 3);
	EXPECT_FALSE(thread.notified());
	EXPECT_EQ(thread.notify(), 0);
	EXPECT_TRUE(thread.notified());
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 3);
	variable = 4;
	platform.yield(platform.frequency());
	EXPECT_EQ(variable, 4);
}

struct ThreadExit : public Thread {
	virtual void * run() {
		MyCriticalSection guard(myMutex);
		exit();
		return 0;
	}
};

TEST_F(ThreadTest, Exit) {
	ThreadExit thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
}

TEST_F(ThreadTest, ReturnJoin) {
	Thread thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(thread.join(), 0);
}

TEST_F(ThreadTest, ExitJoin) {
	Thread thread;
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ(thread.join(), 0);
}

struct ThreadJoinReturn : public Thread {
	virtual void * run() {
		::com::diag::desperado::Platform::instance().yield(::com::diag::desperado::Platform::instance().frequency());
		return 0;
	}
};

TEST_F(ThreadTest, JoinReturn) {
	ThreadJoinReturn thread;
	EXPECT_EQ(thread.start(), 0);
	EXPECT_EQ(thread.join(), 0);
}

static MyMutex sacrificialMutex;

struct ThreadStackUnwind : public Thread {
	virtual void * run() {
		MyCriticalSection guard(sacrificialMutex, false);
		while (true) {
			cancellable();
			yield();
		}
		return 0;
	}
};

TEST_F(ThreadTest, StackUnwind) {
	int status;
	ThreadStackUnwind thread;
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_EQ((status = sacrificialMutex.attempt()), EBUSY);
	if (status == 0) { EXPECT_EQ(sacrificialMutex.end(), 0); }
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_TRUE(thread.cancelled());
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ((status = sacrificialMutex.attempt()), 0);
	if (status == 0) { EXPECT_EQ(sacrificialMutex.end(), 0); }
}

struct ThreadUncancellable : public Thread {
	int & variable;
	explicit ThreadUncancellable(int & shared)
	: variable(shared)
	{
		variable = 1;
	}
	virtual void * run() {
		Uncancellable sentry;
		MyCriticalSection guard(myMutex, false);
		while (!notified()) {
			++variable;
			cancellable();
			yield();
		}
		return 0;
	}
};

TEST_F(ThreadTest, Uncancellable) {
	int variable = 1;
	ThreadUncancellable thread(variable);
	EXPECT_FALSE(thread.cancelled());
	EXPECT_EQ(thread.start(), 0);
	platform.yield(platform.frequency());
	EXPECT_FALSE(thread.cancelled());
	int one = variable;
	EXPECT_EQ(thread.cancel(), 0);
	EXPECT_TRUE(thread.cancelled());
	platform.yield(platform.frequency());
	int two = variable;
	EXPECT_NE(one, two);
	EXPECT_FALSE(thread.notified());
	EXPECT_EQ(thread.notify(), 0);
	EXPECT_TRUE(thread.notified());
	EXPECT_EQ(thread.join(), 0);
}

struct ThreadInstance : public Thread {
	Thread ** here;
	pthread_t * id;
	explicit ThreadInstance(Thread ** hh, pthread_t * ii)
	: here(hh)
	, id(ii)
	{}
	virtual void * run() {
		*here = &Thread::instance();
		*id = Thread::self();
		return 0;
	}
};

TEST_F(ThreadTest, Instance) {
	EXPECT_EQ(pthread_self(), Thread::self());
	EXPECT_EQ(pthread_self(), Thread::instance().getIdentity());
	Thread * here = 0;
	pthread_t id = 0;
	ThreadInstance thread(&here, &id);
	EXPECT_EQ(thread.start(), 0);
	EXPECT_EQ(thread.join(), 0);
	ASSERT_NE(here, (Thread*)0);
	EXPECT_EQ(here, &thread);
	EXPECT_EQ(id, thread.getIdentity());
}

static MyMutex conditionmutex;
static Condition conditionodd;
static Condition conditioneven;

struct ThreadCondition : public Thread {
	int & variable;
	explicit ThreadCondition(int & shared)
	: variable(shared)
	{
		variable = 1;
	}
	virtual void * run() {
		MyCriticalSection guard(conditionmutex);
		while (variable < 100) {
			MaskableLogger::instance().debug("Thread: before %d\n", variable);
			while ((variable % 2) == 0) {
				conditionodd.wait(conditionmutex);
			}
			MaskableLogger::instance().debug("Thread: after %d\n", variable);
			++variable;
			conditioneven.signal();
		}
		return 0;
	}
};

TEST_F(ThreadTest, Condition) {
	int variable = 0;
	EXPECT_EQ(variable, 0);
	ThreadCondition thread(variable);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.start(), 0);
	{
		MyCriticalSection guard(conditionmutex);
		while (variable < 99) {
			MaskableLogger::instance().debug("Main: before %d\n", variable);
			while ((variable % 2) != 0) {
				conditioneven.wait(conditionmutex);
			}
			MaskableLogger::instance().debug("Main: after %d\n", variable);
			++variable;
			conditionodd.signal();
		}
	}
	EXPECT_EQ(thread.join(), 0);
	EXPECT_EQ(variable, 100);
}

static void * functionreturns(void * context) {
	*((int *)context) = 1;
	return context;
}

TEST_F(ThreadTest, FunctionReturn) {
	void * result = 0;
	int variable = 0;
	Thread thread;
	EXPECT_EQ(variable, 0);
	EXPECT_NE(thread.getFinal(), &variable);
	EXPECT_EQ(thread.start(functionreturns, &variable), 0);
	EXPECT_EQ(thread.join(result), 0);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.getFinal(), &variable);
	EXPECT_EQ(result, &variable);
}

static void * functionexits(void * context) {
	*((int *)context) = 1;
	Thread::exit(context);
	return 0;
}

TEST_F(ThreadTest, FunctionExit) {
	void * result = 0;
	int variable = 0;
	Thread thread;
	EXPECT_EQ(variable, 0);
	EXPECT_NE(thread.getFinal(), &variable);
	EXPECT_EQ(thread.start(functionexits, &variable), 0);
	EXPECT_EQ(thread.join(result), 0);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.getFinal(), &variable);
	EXPECT_EQ(result, &variable);
}

static void * functionpexits(void * context) {
	*((int *)context) = 1;
	::pthread_exit(context);
	return 0;
}

TEST_F(ThreadTest, FunctionPExit) {
	void * result = 0;
	int variable = 0;
	Thread thread;
	EXPECT_EQ(variable, 0);
	EXPECT_NE(thread.getFinal(), &variable);
	EXPECT_EQ(thread.start(functionpexits, &variable), 0);
	EXPECT_EQ(thread.join(result), 0);
	EXPECT_EQ(variable, 1);
	EXPECT_EQ(thread.getFinal(), &variable);
	EXPECT_EQ(result, &variable);
}

TEST_F(ThreadTest, MemoryBarrier) {
	MemoryBarrier fence;
}

static MyMutex monitormutex;
static Condition monitorcondition;

struct ThreadMonitor : public Thread {
	int & variable;
	explicit ThreadMonitor(int & shared)
	: variable(shared)
	{}
	virtual void * run() {
		::com::diag::desperado::Platform::instance().yield(::com::diag::desperado::Platform::instance().frequency());
		{
			MyCriticalSection guard(monitormutex);
			monitorcondition.signal();
			if (variable == 0) { variable = 1; }
		}
		return 0;
	}
};

TEST_F(ThreadTest, Monitor) {
	int variable = 0;
	ThreadMonitor thread(variable);
	EXPECT_EQ(thread.start(), 0);
	{
		MyCriticalSection guard(monitormutex);
		monitorcondition.wait(monitormutex);
		if (variable == 0) { variable = 2; }
	}
	EXPECT_EQ(thread.join(), 0);
	MaskableLogger::instance().configuration("ThreadTest.Monitor: looks like a %s monitor to me.\n", (variable == 1) ? "Mesa" : (variable == 2) ? "Hoare" : "Unknown");
}

static MyMutex parallelmutex;
static Condition parallelcondition;

struct ThreadParallel : public Thread {
	uint64_t & enabled;
	uint64_t mask;
	uint64_t & variable;
	explicit ThreadParallel(uint64_t & ee, int nn, uint64_t & vv)
	: enabled(ee)
	, mask(1ULL << nn)
	, variable(vv)
	{}
	virtual void * run() {
		MyCriticalSection guard(parallelmutex);
		while ((enabled & mask) == 0) {
			parallelcondition.wait(parallelmutex);
		}
		if ((variable & mask) == 0) {
			variable |= mask;
		} else {
			variable &= ~mask;
		}
		return 0;
	}
};

TEST_F(ThreadTest, Parallel) {
	uint64_t enabled = 0;
	uint64_t variable = 0;
	ThreadParallel * thread[64];
	for (int index = 0; index < countof(thread); ++index) {
		thread[index] = new ThreadParallel(enabled, index, variable);
		ASSERT_NE(thread[index], (Thread *)0);
	}
	EXPECT_EQ(variable, 0ULL);
	{
		MyCriticalSection guard(parallelmutex);
		for (int index = 0; index < countof(thread); ++index) {
			EXPECT_EQ(thread[index]->start(), 0);
		}
	}
	EXPECT_EQ(variable, 0ULL);
	{
		for (int index = 0; index < countof(thread); ++index) {
			MyCriticalSection guard(parallelmutex);
			enabled |= 1ULL << index;
			EXPECT_EQ(parallelcondition.signal(), 0);
		}
	}
	for (int index = 0; index < countof(thread); ++index) {
		EXPECT_EQ(thread[index]->join(), 0);
	}
	EXPECT_EQ(variable, 0xffffffffffffffffULL);
	for (int index = 0; index < countof(thread); ++index) {
		delete thread[index];
	}
}

}
}
}

#endif
