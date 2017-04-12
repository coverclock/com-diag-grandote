/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_PACKET_
#define _H_COM_DIAG_DESPERADO_PACKET_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/string.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/InputOutput.h"

namespace com {
namespace diag {
namespace desperado {

/**
 * PacketData is an object that can be appended or prepended to a Packet.
 * It contains a pointer to a user provided data structure that it does not
 * take. It is constructed in the full state so that new data cannot be appended
 * or prepended to it unless existing data is consumed.
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketData {

public:

	/**
	 * Specifies the default allocation size in octets for those derived classes
	 * where dynamic memory allocation is used for buffers.
	 */
	static const size_t ALLOCATION = 1024;

	/**
	 * When used as a fraction, indicates that the object is to be initialized
	 * for prepending.
	 */
	static const size_t PREPEND = 1;

	/**
	 * When used as a fraction, indicates that the object is to initialized for
	 * both appending and prepending.
	 */
	static const size_t EITHER = 2;

	/**
	 * When used as a fraction, indicates that the object is to be initialized
	 * for appending.
	 */
	static const size_t APPEND = intmaxof(size_t);

	/**
	 * This specifies the type of user provided data arrays.
	 */
	typedef uint8_t Datum;

	/**
	 * Ctor.
	 *
	 * @param dp points to the user data object.
	 * @param ve is the size of the user data object in octets.
	 * @param vf specifies how the user data object is to be initialized:
	 *        for append, prepend, or either.
	 */
	explicit PacketData(void * dp /* UNTAKEN */, size_t ve, size_t vf = EITHER)
	: next(0)
	, payload(static_cast<Datum*>(dp))
	, head(payload)
	, tail(payload + ve)
	, extent(ve)
	, fraction(vf)
	{}

	/**
	 * Dtor.
	 */
	virtual ~PacketData() {}

	/**
	 * Append as much of the user data as the suffix space allows. Octets are
	 * copied from the user data front to back.
	 *
	 * @param data points to the user data.
	 * @param length is the length of the data to append in octets.
	 * @return the actual number of octets appended.
	 */
	size_t append(const void * data /* COPIED */, size_t length);

	/**
	 * Prepend as much of the user data as the preface space allows. Octets are
	 * copied from the user data back to front.
	 *
	 * @param data points to the user data.
	 * @param length is the length of the data to prepend in octets.
	 * @return the actual number of octets prepended.
	 */
	size_t prepend(const void * data /* COPIED */, size_t length);

	/**
	 * Consume no more than the specified length of data if it is available.
	 * Data is consumed from front to back and copied into the buffer.
	 *
	 * @param buffer points to the user buffer.
	 * @param length is the length of the data to consume in octets.
	 * @return the actual number of octets consumed.
	 */
	size_t consume(void * buffer, size_t length);

	/**
	 * Consume no more than the specified length of data if it is available.
	 * Data is consumed from front to back and discarded.
	 *
	 * @param length is the length of the data to consume in octets.
	 * @return the actual number of octets consumed.
	 */
	size_t consume(size_t length) { return consume(0, length); }

	/**
	 * Points to the next PacketData in a linked list.
	 */
	PacketData * next;

protected:

	/**
	 * Points to the first octet of the buffer.
	 * Never altered after construction.
	 */
	Datum * const payload;

private:

	/**
	 * The size of the buffer in octets.
	 * Never altered after construction.
	 */
	const size_t extent;

	/**
	 * The amount of slack to leave at the beginning of the buffer when the
	 * first octet is appended or prepended in terms of a fraction of the
	 * remaining unused space.
	 * Never altered after construction.
	 * (>remaining): leave no the space at the beginning (default).
	 * (2): place the first appended or prepended data smack in the middle.
	 * (1): leave all space at the beginning.
	 * (0): wackiness ensues.
	 */
	const size_t fraction;

	/**
	 * Points to the first used octet in the buffer.
	 * (head==0): the buffer is empty; otherwise
	 * (head-payload): number of unused octets at the front of the buffer.
	 * (head==payload): no unused octets at the front of the buffer.
	 * (tail-head): the number of used octets in the buffer.
	 */
	Datum * head;

	/**
	 * Points to the first unused octet just past last used octet in the buffer.
	 * (tail==0): the buffer is empty; otherwise
	 * (payload+extent-tail): number of unused octets at the rear of the buffer.
	 * ((payload+extent)==tail): no unused octets at the rear of the buffer.
	 * (tail-head): the number of used octets in the buffer.
	 */
	Datum * tail;

public:

	/**
	 * Return true if the object is empty, false otherwise.
	 *
	 * @return true if the object is empty, false otherwise.
	 */
	bool empty() const { return (head == 0); }

	/**
	 * Return the total extent of the object in octets. This is always equal to
	 * prefix() plus length() plus suffix().
	 *
	 * @return the total length of the data storage in octets.
	 */
	size_t size() const { return extent; }

	/**
	 * Return a pointer to the beginning of user data in the object. NULL is
	 * returned if the object is empty.
	 *
	 * @return a pointer to the beginning of user data in the object or NULL.
	 */
	const void * buffer() const { return head; }

	/**
	 * Return the number of octets available to be consumed.
	 *
	 * @return the number of octets available to be consumed.
	 */
	size_t length() const { return tail - head; }

	/**
	 * Return the number of octets available to be prepended.
	 *
	 * @return the number of octets available to be prepended.
	 */
	size_t prefix() const { return (head == 0) ? extent : head - payload; }

	/**
	 * Return the number of octets available to be appended.
	 *
	 * @return the number of octets available to be eppended.
	 */
	size_t suffix() const { return (tail == 0) ? extent : payload + extent - tail; }

	/**
	 * Return the object to the empty state.
	 */
	void clear() { head = 0; tail = 0; }

private:

    /**
     *  Copy constructor.
     *
     *  @param that refers to an R-value object of this type.
     */
    PacketData(const PacketData& that);

    /**
     *  Assignment operator.
     *
     *  @param that refers to an R-value object of this type.
     */
    PacketData& operator=(const PacketData& that);

};

/**
 * PacketDataDynamic is an object that can be appended or prepended to a
 * Packet. It contains a pointer to a user provided data array that it takes and
 * deletes upon destruction. It is constructed in the full state so that new
 * data cannot be appended or prepended to it unless existing data is consumed.
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketDataDynamic : public PacketData {

public:

	/**
	 * Ctor.
	 *
	 * @param dp points to the dynamically allocated user data array.
	 * @param ve is the size of the dynamically allocated user data array in
	 *        octets.
	 * @param vf specifies how the dynamically allocated user data array is to
	 *        be initialized, for append, prepend, or either.
	 */
	explicit PacketDataDynamic(Datum * dp /* TAKEN */, size_t ve, size_t vf = EITHER)
	: PacketData(dp, ve, vf)
	{}

	/**
	 * Dtor. The dynamically acquired data array is deleted.
	 */
	virtual ~PacketDataDynamic() { delete [] payload; }

};

/**
 * PacketBuffer is an object that can be appended or prepended to a Packet.
 * It contains a pointer to a user provided data structure that it does not
 * take. It is constructed in the empty state so that new data can be appended
 * or prepended to it. A PacketBuffer can be used independently of a Packet as
 * a dual-ended buffer.
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketBuffer : public PacketData {

public:

	/**
	 * Ctor.
	 *
	 * @param bp points to the user buffer object.
	 * @param ve is the size of the buffer object in octets.
	 * @param vf specifies how the user buffer object is to be initialized:
	 *        for append, prepend, or either.
	 */
	explicit PacketBuffer(void * bp /* UNTAKEN */, size_t ve, size_t vf = EITHER)
	: PacketData(bp, ve, vf)
	{ clear(); }

	/**
	 * Dtor.
	 */
	virtual ~PacketBuffer() {}

};

/**
 * PacketBufferDynamic is an object that can be appended or prepended to a
 * Packet. It contains a pointer to a data array, either user provided that it
 * takes, or self-allocated, and deletes upon destruction. It is constructed
 * in the empty state so that new data can be appended or prepended to it.
 * A PacketBufferDynamic can be used independently of a Packet as a dual-ended
 * buffer.
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketBufferDynamic : public PacketBuffer {

public:

	/**
	 * Ctor.
	 *
	 * @param bp points to the dynamically allocated user buffer array.
	 * @param ve is the size of the dynamically allocated user buffer array in
	 *        octets.
	 * @param vf specifies how the dynamically allocated user buffer array is
	 *        to be initialized: for append, prepend, or either.
	 */
	explicit PacketBufferDynamic(Datum * bp /* TAKEN */, size_t ve, size_t vf = EITHER)
	: PacketBuffer(bp, ve, vf)
	{}

	/**
	 * Ctor. Dynamically allocates its own buffer array.
	 *
	 * @param ve is the size of the dynamically allocated buffer array in
	 *        octets.
	 * @param vf specifies how the dynamically allocated buffer array is
	 *        to be initialized: for append, prepend, or either.
	 */
	explicit PacketBufferDynamic(size_t ve = ALLOCATION, size_t vf = EITHER)
	: PacketBuffer(new Datum[ve], ve, vf)
	{}

	/**
	 * Dtor. The dynamically acquired buffer array is deleted.
	 */
	virtual ~PacketBufferDynamic() { delete [] payload; }

};

class Packet;

/**
 * PacketInput implements an Input functor for a Packet.
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketInput : public Input {

public:

    /**
     * Ctor.
     *
     * @param rp refers to the Packet for this functor.
     */
    explicit PacketInput(Packet & rp /* UNTAKEN */)
    : packet(rp)
    {}

    /**
     * Dtor.
     */
    virtual ~PacketInput() {}

	/**
	 * Return the number of octets available to be consumed.
	 *
	 * @return the number of octets available to be consumed.
	 */
    size_t getLength() const;

    /**
     * Returns the next character.
     *
     * @return a character in an integer if successful, EOF otherwise.
     */
    virtual int operator() ();

    /**
     * Pushes an character in an integer back to the input to be
     * returned on the next call to the input character functor.
     * Only one pushed back character is guaranteed to succeed in
     * between input character operations. The character
     * pushed back does not have to be the previous character input,
     * or even any character that was ever input, although at least
     * one character must have been previously input.
     *
     * @param c is the character to push back into the input.
     * @return the pushed back character is successful, EOF otherwise.
     */
    virtual int operator() (int c);

    /**
     * Inputs a newline terminated line into the buffer of
     * the specified size. If a newline is encountered, it is input
     * into the buffer. Guarantees that the buffer is NUL terminated
     * if it is at least one octet in size. Guarantees that no more
     * than the specified number of octets are returned.
     *
     * @param buffer points to the buffer.
     * @param size is the size of the buffer in octets. Size should be no
     *        larger than the largest possible signed integer.
     * @return the number of octets input (which may be zero) including the
     *         terminating NUL, if successful, EOF otherwise.
     */
    virtual ssize_t operator() (char * buffer, size_t size);

    /**
     * Inputs binary data into a buffer. The intent is that the
     * operation blocks until the minimum number of requested octets
     * are input or EOF or an error occurs. If more octets are available
     * without blocking, up to the maximum number of requested octets
     * are input. Specific implementations may differ. The interface
     * only guarantees that no more than the maximum number of octets
     * a input into the buffer. The functor does not NUL terminate the buffer.
     *
     * If minimum is zero, zero is returned instead of EOF if the underlying
     * data structure is empty. This allows the application to poll for data
     * and for a Packet to be used as a ring buffer.
     *
     * @param buffer  points to the buffer.
     * @param minimum is the minimum number of octets to input.
     * @param maximum is the maximum number of octets to input.
     * @return the number of octets input (which may be any number less
     *         than maximum including zero) if successful, EOF otherwise.
     */
    virtual ssize_t operator() (void * buffer, size_t minimum, size_t maximum);

    /**
     * Displays internal information about this object to the specified
     * output object. Useful for debugging and troubleshooting.
     *
     * @param level sets the verbosity of the output. What this means
     *        is object dependent. However, the level is passed from outer to
     *        inner objects this object calls the show methods of its inherited
     *        or composited objects.
     * @param display points to the output object to which output is sent. If
     *        null (zero), the default platform output object is used as the
     *        effective output object. The effective output object is passed
     *        from outer to inner objects as this object calls the show methods
     *        of its inherited and composited objects.
     * @param indent specifies the level of indentation. One more than this
     *        value is passed from outer to inner objects as this object calls
     *        the show methods of its inherited and composited objects.
     */
    virtual void show(int level = 0, com::diag::desperado::Output * display = 0, int indent = 0) const;

private:

    /**
     * This refers to the Packet for which this object is an input functor.
     */
    Packet& packet;

};

/**
 * PacketOutput implements an Output functor for a Packet.
 * @author coverclock@diag.com (Chip Overclock)
 */
class PacketOutput : public Output {

public:

    /**
     * Ctor.
     *
     * @param rp refers to the Packet for this functor.
     */
    explicit PacketOutput(Packet & rp /* UNTAKEN */)
    : packet(rp)
    {}

    /**
     * Dtor.
     */
    virtual ~PacketOutput() {}

    /**
     * Outputs a character in integer form.
     *
     * @param c is a character in integer form.
     * @return the output character if successful, EOF otherwise.
     */
    virtual int operator() (int c);

    /**
     * Outputs a string of no more than the specified length not including
     * its terminating NUL.
     *
     * @param s points to a constant NUL-terminated string.
     * @param size is the size of the string in octets.
     * @return the number of octets output if successful (which may be zero),
     *         EOF otherwise.
     */
    virtual ssize_t operator() (const char * s /* COPIED */, size_t size = com::diag::desperado::Output::maximum_string_length);

    /**
     * Format a variable length argument list and output the result.
     *
     * @param format is a NUL-terminated string containing a printf-style format
     *        statement.
     * @param ap is a variable length argument object.
     * @return a non-negative number if successful, EOF otherwise.
     */
    virtual ssize_t operator() (const char * format, va_list ap);

    /**
     * Outputs binary data from a buffer. The intent is that the
     * operation blocks until the minimum number of requested octets
     * are output or EOF or an error occurs. If more space is available
     * without blocking, up to the maximum number of requested octets
     * may be output. Specific implementations may differ. The interface
     * only guarantees that at least the minimum number of octets are
     * output from the buffer.
     *
     * @param buffer points to the buffer.
     * @param minimum is the minimum number of octets to output.
     * @param maximum is the maximum number of octets to output.
     * @return the number of octets output (which may be any number less
     *         than maximum including zero) if successful, EOF otherwise.
     */
    virtual ssize_t operator() (const void * buffer /* COPIED */, size_t minimum, size_t maximum);

    /**
     * Flush any buffered data to the file. Specific implementations may do
     * nothing if they do not buffer.
     *
      * @return a non-negative number if successful, EOF otherwise.
     */
    virtual int operator() ();

    /**
     * Displays internal information about this object to the specified
     * output object. Useful for debugging and troubleshooting.
     *
     * @param level sets the verbosity of the output. What this means
     *        is object dependent. However, the level is passed from outer to
     *        inner objects this object calls the show methods of its inherited
     *        or composited objects.
     * @param display points to the output object to which output is sent. If
     *        null (zero), the default platform output object is used as the
     *        effective output object. The effective output object is passed
     *        from outer to inner objects as this object calls the show methods
     *        of its inherited and composited objects.
     * @param indent specifies the level of indentation. One more than this
     *        value is passed from outer to inner objects as this object calls
     *        the show methods of its inherited and composited objects.
     */
    virtual void show(int level = 0, com::diag::desperado::Output * display = 0, int indent = 0) const;

private:

    /**
     * This refers to the Packet for which this object is an output functor.
     */
    Packet& packet;

};

/**
 * Packet is a container of PacketData, PacketDataDynamic, PacketBuffer,
 * and/or PacketBufferDynamic objects to which data can be prepended and
 * appended. Data can be appended using an Output functor and consumed using
 * an Input functor. Appropriate use of the various PacketData and PacketBuffer
 * objects can allow layered communications prototol packets to be built
 * dynamically without a lot of copying. If you are very clever, you can even
 * insert data in the middle of a Packet by inserting empty or only partially
 * filled objects into the packet.
 * @author coverclock@diag.com (Chip Overclock)
 */
class Packet : public InputOutput {

public:

	/**
	 * Specifies the default allocation size in octets when dynamically
	 * allocating PacketBufferDynamic buffer space.
	 */
	static const size_t ALLOCATION = 4096;

	/**
	 * When used as a fraction, indicates that the first allocated PacketBuffer
	 * is to be optimized for prepending.
	 */
	static const size_t PREPEND = PacketBufferDynamic::PREPEND;

	/**
	 * When used as a fraction, indicates that the first allocated PacketBuffer
	 * is to allow both appending and prepending.
	 */
	static const size_t EITHER = PacketBufferDynamic::EITHER;

	/**
	 * When used as a fraction, indicates that the first allocated PacketBuffer
	 * is to be optimized for appending.
	 */
	static const size_t APPEND = PacketBufferDynamic::APPEND;

    /**
     *  Ctor.
     *
     *  @param va is the default allocation size in octets.
     *  @param vf is the fraction of the very first allocation.
     */
    explicit Packet(size_t va = ALLOCATION, size_t vf = APPEND)
    : allocation(va)
    , fraction(vf)
    , head(0)
    , tail(0)
    , in(*this)
    , out(*this)
    {}

    /**
     *  Destructor.
     */
    virtual ~Packet() { clear(); }

    /**
     *  Returns a reference to the input functor interface.
     *
     *  @return a reference to the input functor interface.
     */
    virtual PacketInput & input() { return in; }

    /**
     *  Returns a reference to the output functor interface.
     *
     *  @return a reference to the output functor interface.
     */
    virtual PacketOutput & output() { return out; }

    /**
     * Returns true if the object is empty, false otherwise. Empty means the
     * object has nothing on its linked list. It is not empty even if the
     * items on its linked list are empty.
     *
     * @return true if the object is empty, false otherwise.
     */
    bool empty() const { return (head == 0); }

    /**
     * Return the object to its empty state.
     */
    void clear();

    /**
     * Append the PacketData (or derivative) onto the object.
     *
     * @param rd refers to a PacketData (or derivative).
     */
	void append(PacketData & rd /* TAKEN */);

    /**
     * Prepend the PacketData (or derivative) onto the object.
     *
     * @param rd refers to a PacketData (or derivative).
     */
	void prepend(PacketData & rd /* TAKEN */);

	/**
	 * Append the data by copying it, allocating new PacketBufferDynamic objects
	 * and appended them as needed. Data is copied from front to back.
	 *
	 * @param data points to the data.
	 * @param length is the length of the data to append in octets.
	 * @return the number of octets appended.
	 */
	size_t append(const void * data /* COPIED */, size_t length);

	/**
	 * Prepend the data by copying it, allocating new PacketBufferDynamic
	 * objects and prepending them as needed. Data is copied from back to front.
	 *
	 * @param data points to the data.
	 * @param length is the length of the data to prepend in octets.
	 * @return the number of octets appended.
	 */
	size_t prepend(const void * data /* COPIED */, size_t length);

	/**
	 * Consume no more than the specified length of data if it is available.
	 * Data is consumed front to back and copied into the buffer.
	 *
	 * @param buffer points to the buffer.
	 * @param length is the length of the data to consume in octets.
	 * @return the number of octets consumed.
	 */
	size_t consume(void * buffer, size_t length);

	/**
	 * Consume no more than the specified length of data if it is available.
	 * Data is consumed front to back and discarded.
	 *
	 * @param length is the length of the data to consume in octets.
	 * @return the number of octets consumed.
	 */
	size_t consume(size_t length) { return consume(0, length); }

	/**
	 * Tranfers the contents of an input functor into this object using an
	 * algorithm that minimizes memory to memory copying until the input is
	 * empty or an error occurs.
	 *
	 * @param from refers to the input functor.
	 * @return the number of octets transferred.
	 */
	size_t source(Input& from);

	/**
	 * Tranfers the contents of this object into an output functor using an
	 * algorithm that minimizes memory to memory copying until the object is
	 * empty or an error occurs.
	 *
	 * @param to refers to the output functor.
	 * @return the number of octets transferred.
	 */
	size_t sink(Output& to);

	/**
	 * Return the number of octets available to be consumed. This is done at
	 * every call by traversing the entire linked list and summing the length
	 * of each individual PacketData.
	 *
	 * @return the number of octets available to be consumed.
	 */
	size_t length() const;

    /**
     * Displays internal information about this object to the specified
     * output object. Useful for debugging and troubleshooting.
     *
     * @param level sets the verbosity of the output. What this means
     *        is object dependent. However, the level is passed from outer to
     *        inner objects this object calls the show methods of its inherited
     *        or composited objects.
     * @param display points to the output object to which output is sent. If
     *        null (zero), the default platform output object is used as the
     *        effective output object. The effective output object is passed
     *        from outer to inner objects as this object calls the show methods
     *        of its inherited and composited objects.
     * @param indent specifies the level of indentation. One more than this
     *        value is passed from outer to inner objects as this object calls
     *        the show methods of its inherited and composited objects.
     */
    virtual void show(int level = 0, Output * display = 0, int indent = 0) const;

private:

    /**
     * This is the number of octets allocated for data storage per
     * PacketBufferDynamic.
     */
    const size_t allocation;

    /**
     * This is the fraction used to initialize the very first
     * PacketBufferDynamic for appending, prepending, or both.
     */
	const size_t fraction;

	/**
	 * Points to the first object on the linked list or NULL if empty.
	 */
    PacketData * head;

	/**
	 * Points to the last object on the linked list or NULL if empty.
	 */
    PacketData * tail;

    /**
     *  This is the Input functor to the Packet.
     */
    PacketInput in;

    /**
     *  This is the Output functor to the Packet.
     */
    PacketOutput out;

private:

    /**
     *  Copy ctor.
     *
     *  @param that refers to an R-value object of this type.
     */
    Packet(const Packet & that);

    /**
     *  Assignment operator.
     *
     *  @param that refers to an R-value object of this type.
     */
    Packet& operator=(const Packet & that);

};

// This is here because the type of packet isn't fully defined during the
// class declaration.
inline size_t PacketInput::getLength() const { return packet.length(); }

}
}
}

#endif
