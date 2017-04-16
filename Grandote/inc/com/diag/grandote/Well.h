/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_GRANDOTE_WELL_
#define _H_COM_DIAG_GRANDOTE_WELL_

/**
 * @file
 *
 * Copyright 2013-2017 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * http://www.diag.com/navigation/downloads/Grandote.html<BR>
 *
 * This is a C++ wrapper around the Diminuto well facility.
 */

#if defined(__cplusplus)
extern "C" {
#include "com/diag/diminuto/diminuto_well.h"
#include "com/diag/diminuto/diminuto_platform.h"
}
#endif

#include <pthread.h>
#include <sys/types.h>

#if !defined(COM_DIAG_GRANDOTE_PLATFORM_BIONIC)
#	include <exception>
#endif
#include <new>

namespace com {
 namespace diag {
  namespace grandote {

/**
 * This is a base Well class.
 */
class BaseWell {

public:

	/**
	 * Destructor.
	 * If the well was lazily allocated, it is freed at this time. Any
	 * objects of allocated from the well will have their
	 * memory deallocated as well.
	 */
	virtual ~BaseWell();

	/**
	 * Allocate memory for the well. The cardinality and alignment of of
	 * objects of type _TYPE_ in the well were specified at construction.
	 */
	void init();

	/**
	 * Deallocate memory for the well if and only iff there are no objects
	 * of type _TYPE_ allocated from the well.
	 */
	void fini();

	/**
	 * Return true is the well is full, that is, no objects of type _TYPE_
	 * are currently allocated from it.
	 * @return true if the well is full, false otherwise.
	 */
	bool isFull() const;

	/**
	 * Return true if the well is empty, that is, the next new operator
	 * will return a null pointer.
	 * @return true if the well is empty, false otherwise.
	 */
	bool isEmpty() const;

protected:

	/**
	 * Constructor.
	 * [1] If cardinality is zero, memory for the well must be explicitly
	 * allocated by the application by calling the
	 * init(cardinality, alignment) method with appropriate values sometime
	 * after construction and prior to the first attempt at allocating an
	 * object of type _TYPE_ from the well.
	 * [2] If cardinality is greater than zero but preallocation is not
	 * enabled, memory for the well will be allocated lazily on the first
	 * attempt at calling the _TYPE_::new() operator. Hence if the well is
	 * never used, memory is never allocated for it. However, that first
	 * call to _TYPE_::new() is going to be more expensive than subsequent
	 * calls.
	 * [3] If cardinality is greater than zero and preallocation is enabled,
	 * memory for the well is allocated at the time of well construction.
	 * @param ss is the size of objects in the well in bytes.
	 * @param cc is the fixed number of objects in the well.
	 * @param mm if true causes memory to be allocated during construction.
	 * @param aa is the alignment of objects in the well.
	 * @param pp is the virtual page size of the underlying platform or zero.
	 * @param ll is the cache line size of the underlying target or zero.
	 */
	explicit BaseWell(size_t ss, size_t cc, bool mm, size_t aa, size_t pp = 0, size_t ll = 0);

	/**
	 * Allocate memory for the well.
	 * @param ss is the size of an object allocated from the well.
	 * @param cc is the count of objects in the well.
	 * @param aa is the alignment of objects in the well.
	 * @param pp is the virtual page size of the underlying platform or zero.
	 * @param ll is the cache line size of the underlying target or zero.
	 */
	void init(size_t ss, size_t cc, size_t aa, size_t pp = 0, size_t ll = 0);

	/**
	 * Allocate memory for an object of type _TYPE_ from the well.
	 * @return a pointer to the memory allocated from the well.
	 */
	void * alloc();

	/**
	 * Free memory for an object of type _TYPE_ back to the well.
	 * @param pointer points to the memory to be freed.
	 */
	void free(void * pointer);

	/**
	 * This is the size of an object allocated in the well.
	 */
	size_t size;

	/**
	 * This is the number of objects of type _TYPE_ whose memory will be
	 * allocated in the well.
	 */
	size_t cardinality;

	/**
	 * This is the alignment of the memory for each object of type _TYPE_ in
	 * the well. For example, if each object is eight-byte aligned, this
	 * value would be eight.
	 */
	size_t alignment;

	/**
	 * This is the virtual page size of the underlying platform, or zero
	 * to use the platform default.
	 */
	size_t pagesize;

	/**
	 * This is the cache line size of the underlying target, or zero to use
	 * the platform default.
	 */
	size_t linesize;

	/**
	 * This is a pointer to the lazily allocated well for objects of type
	 * _TYPE_. Underneath the hood, this is really an array of Diminuto
	 * list structures.
	 */
	diminuto_well_t * wellp;

};

/**
 * This is the thread-safe base Well class.
 */
class SafeBaseWell : public BaseWell {

public:

	/**
	 * Destructor.
	 * If the well was lazily allocated, it is freed at this time. Any
	 * objects of allocated from the well will have their
	 * memory deallocated as well.
	 */
	virtual ~SafeBaseWell();

	/**
	 * Allocate memory for the well. The cardinality and alignment of of
	 * objects of type _TYPE_ in the well were specified at construction.
	 */
	void init();

	/**
	 * Deallocate memory for the well if and only iff there are no objects
	 * of type _TYPE_ allocated from the well.
	 */
	void fini();

protected:

	/**
	 * Constructor.
	 * [1] If cardinality is zero, memory for the well must be explicitly
	 * allocated by the application by calling the
	 * init(cardinality, alignment) method with appropriate values sometime
	 * after construction and prior to the first attempt at allocating an
	 * object of type _TYPE_ from the well.
	 * [2] If cardinality is greater than zero but preallocation is not
	 * enabled, memory for the well will be allocated lazily on the first
	 * attempt at calling the _TYPE_::new() operator. Hence if the well is
	 * never used, memory is never allocated for it. However, that first
	 * call to _TYPE_::new() is going to be more expensive than subsequent
	 * calls.
	 * [3] If cardinality is greater than zero and preallocation is enabled,
	 * memory for the well is allocated at the time of well construction.
	 * @param ss is the size of objects in the well in bytes.
	 * @param cc is the fixed number of objects in the well.
	 * @param mm if true causes memory to be allocated during construction.
	 * @param aa is the alignment of objects in the well.
	 * @param pp is the virtual page size of the underlying platform or zero.
	 * @param ll is the cache line size of the underlying target or zero.
	 */
	explicit SafeBaseWell(size_t ss, size_t cc, bool mm, size_t aa, size_t pp = 0, size_t ll = 0);

	/**
	 * Allocate memory for the well.
	 * @param ss is the size of an object allocated from the well.
	 * @param cc is the count of objects in the well.
	 * @param aa is the alignment of objects in the well.
	 * @param pp is the virtual page size of the underlying platform or zero.
	 * @param ll is the cache line size of the underlying target or zero.
	 */
	void init(size_t ss, size_t cc, size_t aa, size_t pp, size_t ll);

	/**
	 * Allocate memory for an object of type _TYPE_ from the well.
	 * @return a pointer to the memory allocated from the well.
	 */
	void * alloc();

	/**
	 * Free memory for an object of type _TYPE_ back to the well.
	 * @param pointer points to the memory to be freed.
	 */
	void free(void * pointer);

	/**
	 * This is a POSIX thread mutex that makes operations on the well
	 * thread-safe.
	 */
	pthread_mutex_t mutex;

};

template <class _TYPE_, size_t _ALIGNMENT_ = sizeof(uint64_t)>
/**
 * This is a templated Well class for objects of type _TYPE_. When a new()
 * operator is called for an object of type _TYPE_, the memory for the
 * object is allocated from the well. However, the object is not pre-
 * constructed. Instead, C++ calls the _TYPE_ constructor at allocation
 * time. The well is allocated lazily when the first object of type _TYPE_
 * is allocated. This prevents memory from being allocation for objects
 * that are never used.
 */
class Well : public BaseWell {

public:

	/**
	 * Constructor.
	 * [1] If cardinality is zero, memory for the well must be explicitly
	 * allocated by the application by calling the
	 * init(cardinality, alignment) method with appropriate values sometime
	 * after construction and prior to the first attempt at allocating an
	 * object of type _TYPE_ from the well.
	 * [2] If cardinality is greater than zero but preallocation is not
	 * enabled, memory for the well will be allocated lazily on the first
	 * attempt at calling the _TYPE_::new() operator. Hence if the well is
	 * never used, memory is never allocated for it. However, that first
	 * call to _TYPE_::new() is going to be more expensive than subsequent
	 * calls.
	 * [3] If cardinality is greater than zero and preallocation is enabled,
	 * memory for the well is allocated at the time of well construction.
	 * @param cc is the fixed number of items of type _TYPE_ in the well.
	 * @param mm if true causes memory to be allocated during construction.
	 * @param aa is the alignment of objects in the well.
	 * @param pp is the virtual page size of the underlying platform or zero.
	 * @param ll is the cache line size of the underlying target or zero.
	 */
	explicit Well(size_t cc = 0, bool mm = true, size_t aa = _ALIGNMENT_, size_t pp = 0, size_t ll = 0)
	: BaseWell(sizeof(_TYPE_), cc, mm, aa, pp, ll)
	{}

	/**
	 * Destructor.
	 * If the well was lazily allocated, it is freed at this time. Any
	 * objects of type _TYPE_ allocated from the well will have their
	 * memory deallocated as well.
	 */
	virtual ~Well() {}

	using BaseWell::init;

	/**
	 * Allocate memory for the well.
	 * @param cc is the count of objects of type _TYPE_ in the well.
	 * @param aa is the alignment of objects of type _TYPE_ in the well.
	 * @param pp is the virtual page size of the underlying platform or zero.
	 * @param ll is the cache line size of the underlying target or zero.
	 */
	void init(size_t cc, size_t aa = _ALIGNMENT_, size_t pp = 0, size_t ll = 0) { BaseWell::init(sizeof(_TYPE_), cc, aa, pp, ll); }

	/**
	 * Allocate memory for an object of type _TYPE_ from the well.
	 * @return a pointer to the memory allocated from the well.
	 */
	_TYPE_ * alloc() { return static_cast<_TYPE_ *>(BaseWell::alloc()); }

	/**
	 * Free memory for an object of type _TYPE_ back to the well.
	 * @param pointer points to the memory to be freed.
	 */
	void free(_TYPE_ * pointer) { BaseWell::free(pointer); }

};

template <class _TYPE_, size_t _ALIGNMENT_ = sizeof(uint64_t)>
/**
 * This is a templated Well class for objects of type _TYPE_. When a new()
 * operator is called for an object of type _TYPE_, the memory for the
 * object is allocated from the well. However, the object is not pre-
 * constructed. Instead, C++ calls the _TYPE_ constructor at allocation
 * time. The well is allocated lazily when the first object of type _TYPE_
 * is allocated. This prevents memory from being allocation for objects
 * that are never used.
 */
class SafeWell : public SafeBaseWell {

public:

	/**
	 * Constructor.
	 * [1] If cardinality is zero, memory for the well must be explicitly
	 * allocated by the application by calling the
	 * init(cardinality, alignment) method with appropriate values sometime
	 * after construction and prior to the first attempt at allocating an
	 * object of type _TYPE_ from the well.
	 * [2] If cardinality is greater than zero but preallocation is not
	 * enabled, memory for the well will be allocated lazily on the first
	 * attempt at calling the _TYPE_::new() operator. Hence if the well is
	 * never used, memory is never allocated for it. However, that first
	 * call to _TYPE_::new() is going to be more expensive than subsequent
	 * calls.
	 * [3] If cardinality is greater than zero and preallocation is enabled,
	 * memory for the well is allocated at the time of well construction.
	 * @param cc is the fixed number of items of type _TYPE_ in the well.
	 * @param mm if true causes memory to be allocated during construction.
	 * @param aa is the alignment of objects of type _TYPE_ in the well.
	 * @param pp is the virtual page size of the underlying platform or zero.
	 * @param ll is the cache line size of the underlying target or zero.
	 */
	explicit SafeWell(size_t cc = 0, bool mm = true, size_t aa = _ALIGNMENT_, size_t pp = 0, size_t ll = 0)
	: SafeBaseWell(sizeof(_TYPE_), cc, pp, aa, pp, ll)
	{}

	/**
	 * Destructor.
	 * If the well was lazily allocated, it is freed at this time. Any
	 * objects of type _TYPE_ allocated from the well will have their
	 * memory deallocated as well.
	 */
	virtual ~SafeWell() {}

	using SafeBaseWell::init;

	/**
	 * Allocate memory for the well.
	 * @param cc is the count of objects of type _TYPE_ in the well.
	 * @param aa is the alignment of objects of type _TYPE_ in the well.
	 * @param pp is the virtual page size of the underlying platform or zero.
	 * @param ll is the cache line size of the underlying target or zero.
	 */
	void init(size_t cc, size_t aa = _ALIGNMENT_, size_t pp = 0, size_t ll = 0) { SafeBaseWell::init(sizeof(_TYPE_), cc, aa, pp, ll); }

	/**
	 * Allocate memory for an object of type _TYPE_ from the well.
	 * @return a pointer to the memory allocated from the well.
	 */
	_TYPE_ * alloc() { return static_cast<_TYPE_ *>(BaseWell::alloc()); }

	/**
	 * Free memory for an object of type _TYPE_ back to the well.
	 * @param pointer points to the memory to be freed.
	 */
	void free(_TYPE_ * pointer) { SafeBaseWell::free(pointer); }

};

  }
 }
}

#if !defined(COM_DIAG_GRANDOTE_PLATFORM_BIONIC)

	/**
	 * @def GRANDOTE_WELL_OPERATOR_DECLARATIONS
	 * Declares the new and delete operators for use inside a class declaration.
	 */
#	define GRANDOTE_WELL_OPERATOR_DECLARATIONS(_TYPE_) \
		static void * operator new(std::size_t size) throw (std::bad_alloc); \
		static void * operator new(std::size_t size, const std::nothrow_t& nothrow) throw(); \
		static void operator delete(void * pointer) throw(); \
		static void operator delete(void * pointer, const std::nothrow_t& nothrow) throw();

	/**
	 * @def GRANDOTE_WELL_OPERATOR_DEFINITIONS
	 * Defines the new and delete operators for use inside a class declaration.
	 */
#	define GRANDOTE_WELL_OPERATOR_DEFINITIONS(_TYPE_) \
		void * _TYPE_::operator new(std::size_t size) throw (std::bad_alloc) { _TYPE_ * pointer = com_diag_diminuto_well.alloc(); if (pointer == static_cast<_TYPE_ *>(0)) { std::bad_alloc oom; throw oom; } return pointer; } \
		void * _TYPE_::operator new(std::size_t size, const std::nothrow_t& nothrow) throw() { return com_diag_diminuto_well.alloc(); } \
		void _TYPE_::operator delete(void * pointer) throw() { com_diag_diminuto_well.free(static_cast<_TYPE_ *>(pointer)); } \
		void _TYPE_::operator delete(void * pointer, const std::nothrow_t& nothrow) throw() { com_diag_diminuto_well.free(static_cast<_TYPE_ *>(pointer)); }

#else

	/**
	 * @def GRANDOTE_WELL_OPERATOR_DECLARATIONS
	 * Declares the new and delete operators for use inside a class declaration.
	 */
#	define GRANDOTE_WELL_OPERATOR_DECLARATIONS(_TYPE_) \
		static void * operator new(std::size_t size); \
		static void * operator new(std::size_t size, const std::nothrow_t& nothrow); \
		static void operator delete(void * pointer); \
		static void operator delete(void * pointer, const std::nothrow_t& nothrow);

	/**
	 * @def GRANDOTE_WELL_OPERATOR_DEFINITIONS
	 * Defines the new and delete operators for use inside a class declaration.
	 */
#	define GRANDOTE_WELL_OPERATOR_DEFINITIONS(_TYPE_) \
		void * _TYPE_::operator new(std::size_t size) { return com_diag_diminuto_well.alloc(); } \
		void * _TYPE_::operator new(std::size_t size, const std::nothrow_t& nothrow) { return com_diag_diminuto_well.alloc(); } \
		void _TYPE_::operator delete(void * pointer) { com_diag_diminuto_well.free(static_cast<_TYPE_ *>(pointer)); } \
		void _TYPE_::operator delete(void * pointer, const std::nothrow_t& nothrow) { com_diag_diminuto_well.free(static_cast<_TYPE_ *>(pointer)); }

#endif

/**
 * @def GRANDOTE_WELL_DECLARACTION
 * Intended to be used inside the class declaration for the class @a _TYPE_.
 * Declares a static well and the static new() and delete() operators used
 * to allocate and free objects of type _CLASS_. You can also just declare
 * these yourself if you choose. If the new operator fails to allocate an
 * object of type _TYPE_ from the well, a std::bad_alloc exception is thrown.
 * if the new(nothrow) operator fails to allocate an object of type _TYPE_ from
 * the well, a null pointer is returned. This form of well is NOT thread-safe.
 */
#define GRANDOTE_WELL_DECLARATION(_TYPE_) \
	static com::diag::grandote::Well<_TYPE_> com_diag_diminuto_well; \
	GRANDOTE_WELL_OPERATOR_DECLARATIONS(_TYPE_)

 /**
  * @def GRANDOTE_WELL_DEFINITION
  * Intended to be used in the translation unit that defines the class @a _TYPE_
  * and its well of @a _CARDINALITY_ objects of type _TYPE_. This defines the
  * objects in the well to have the default alignment. But you can define the
  * well yourself if you want and specify any alignment you choose. This form
  * of well is NOT thread-safe.
  */
 #define GRANDOTE_WELL_DEFINITION(_TYPE_, _CARDINALITY_) \
 	com::diag::grandote::Well<_TYPE_> _TYPE_::com_diag_diminuto_well(_CARDINALITY_); \
 	GRANDOTE_WELL_OPERATOR_DEFINITIONS(_TYPE_)

 /**
  * @def GRANDOTE_SAFEWELL_DECLARACTION
  * Intended to be used inside the class declaration for the class @a _TYPE_.
  * Declares a static well and the static new() and delete() operators used
  * to allocate and free objects of type _CLASS_. You can also just declare
  * these yourself if you choose. If the new operator fails to allocate an
  * object of type _TYPE_ from the well, a std::bad_alloc exception is thrown.
  * if the new(nothrow) operator fails to allocate an object of type _TYPE_ from
  * the well, a null pointer is returned. This form of well is thread-safe.
  */
 #define GRANDOTE_SAFEWELL_DECLARATION(_TYPE_) \
 	static com::diag::grandote::SafeWell<_TYPE_> com_diag_diminuto_well; \
 	GRANDOTE_WELL_OPERATOR_DECLARATIONS(_TYPE_)

 /**
  * @def GRANDOTE_SAFEWELL_DEFINITION
  * Intended to be used in the translation unit that defines the class @a _TYPE_
  * and its well of @a _CARDINALITY_ objects of type _TYPE_. This defines the
  * objects in the well to have the default alignment. But you can define the
  * well yourself if you want and specify any alignment you choose. This form
  * of well is thread-safe.
  */
 #define GRANDOTE_SAFEWELL_DEFINITION(_TYPE_, _CARDINALITY_) \
 	com::diag::grandote::SafeWell<_TYPE_> _TYPE_::com_diag_diminuto_well(_CARDINALITY_); \
 	GRANDOTE_WELL_OPERATOR_DEFINITIONS(_TYPE_)

#endif
