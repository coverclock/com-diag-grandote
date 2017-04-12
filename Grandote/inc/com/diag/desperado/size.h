/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_DESPERADO_SIZE_
#define _H_COM_DIAG_DESPERADO_SIZE_

/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include "com/diag/desperado/target.h"
#include "com/diag/desperado/stdio.h"

namespace com {
namespace diag {
namespace desperado {
class BufferInput;
class BufferOutput;
class DataInput;
class DescriptorInput;
class DescriptorOutput;
class FileInput;
class FileOutput;
class Input;
class Output;
}
}
}

namespace com {
namespace diag {
namespace desperado {

class Packet;
class PacketData;
class PacketInput;

typedef off_t Size;

/**
 * Determine the size of a resource in the file system.
 *
 * @param path is a file system path.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const char * path);

/**
 * Determine the size of a resource represented by a file descriptor.
 *
 * @param fd is an open file descriptor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(int fd);

/**
 * Determine the size of a resource represented by a standard I/O FILE pointer.
 *
 * @param path is a standard I/O FILE pointer.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const FILE * fp);

/**
 * Determine the size of a BufferInput functor.
 *
 * @param input refers to a BufferInput functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const BufferInput & input);

/**
 * Determine the size of a BufferOutput functor.
 *
 * @param output refers to a BufferOutput functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const BufferOutput & output);

/**
 * Determine the size of a DataInput functor.
 *
 * @param input refers to a DataInput functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const DataInput & input);

/**
 * Determine the size of a DescriptorInput functor.
 *
 * @param input refers to a DescriptorInput functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const DescriptorInput & input);

/**
 * Determine the size of a DescriptorOutput functor.
 *
 * @param output refers to a DescriptorOutput functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const DescriptorOutput & output);

/**
 * Determine the size of a DescriptorInput functor.
 *
 * @param input refers to a DescriptorInput functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const FileInput & input);

/**
 * Determine the size of a DescriptorOutput functor.
 *
 * @param output refers to a DescriptorOutput functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const FileOutput & output);

/**
 * Determine the size of a Input functor. (This always fails.)
 *
 * @param input refers to a Input functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const Input & input);

/**
 * Determine the size of an Output functor. (This always fails.)
 *
 * @param output refers to an Output functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const Output & output);

/**
 * Determine the size of a PacketInput functor.
 *
 * @param input refers to a PacketInput functor.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const PacketInput & input);

/**
 * Determine the size of a Packet.
 *
 * @param packet refers to a Packet.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const Packet & packet);

/**
 * Determine the size of a PacketData.
 *
 * @param path refers to a PacketData.
 * @return the size of the resource if it can be determined, otherwise EOF.
 */
Size size(const PacketData & data);

}
}
}

#endif
