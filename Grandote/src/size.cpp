/**
 * @file
 *
 * Copyright 2011 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Desperadito.html<BR>
 */

#include <sys/stat.h>
#include <unistd.h>
#include "com/diag/desperado/size.h"
#include "com/diag/desperado/BufferInput.h"
#include "com/diag/desperado/BufferOutput.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/DescriptorInput.h"
#include "com/diag/desperado/DescriptorOutput.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/Packet.h"

namespace com {
namespace diag {
namespace desperado {

Size size(const char * path) {
	Size result = EOF;
	struct ::stat status;
	if (path == 0) {
		// Do nothing: invalid!
	} else if (::stat(path, &status) < 0) {
		// Do nothing: failed!
	} else if (!S_ISREG(status.st_mode)) {
		// Do nothing: cannot determine!
	} else {
		result = status.st_size;
	}
	return result;
}

Size size(int fd) {
	Size result = EOF;
	struct ::stat status;
	if (fd < 0) {
		// Do nothing: invalid!
	} else if (::fstat(fd, &status) < 0) {
		// Do nothing: failed!
	} else if (!S_ISREG(status.st_mode)) {
		// Do nothing: cannot determine!
	} else {
		result = status.st_size;
	}
	return result;
}

Size size(const FILE * fp) {
	Size result = EOF;
	if (fp == 0) {
		// Do nothing: invalid!
	} else {
		// Remarkably, fileno(3) does not declare its argument to be const.
		// I can't believe it actually alters the FILE structure in any
		// meaningful way. So I'm totally cheating here.
		result = size(::fileno(const_cast<FILE *>(fp)));
	}
	return result;
}

Size size(const BufferInput & input) {
	return input.getLength();
}

Size size(const BufferOutput & output) {
	return output.getLength();
}

Size size(const DataInput & input) {
	return input.getLength();
}

Size size(const DescriptorInput & input) {
	return (size(input.getDescriptor()) + input.getPushed());
}

Size size(const DescriptorOutput & output) {
	return size(output.getDescriptor());
}

Size size(const FileInput & input) {
	return size(input.getFile());
}

Size size(const FileOutput & output) {
	return size(output.getFile());
}

Size size(const Input & input) {
	return EOF;
}

Size size(const Output & output) {
	return EOF;
}

Size size(const PacketInput & input) {
	return input.getLength();
}

Size size(const Packet & packet) {
	return packet.length();
}

Size size(const PacketData & data) {
	return data.length();
}

}
}
}
