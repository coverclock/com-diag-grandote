/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Desperadito library.
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    As a special exception, if other files instantiate templates or
    use macros or inline functions from this file, or you compile
    this file and link it with other works to produce a work based on
    this file, this file does not by itself cause the resulting work
    to be covered by the GNU Lesser General Public License. However
    the source code for this file must still be made available in
    accordance with the GNU Lesser General Public License.

    This exception does not invalidate any other reasons why a work
    based on this file might be covered by the GNU Lesser General
    Public License.

    Alternative commercial licensing terms are available from the copyright
    holder. Contact Digital Aggregates Corporation for more information.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place, Suite 330,
    Boston, MA 02111-1307 USA, or http://www.gnu.org/copyleft/lesser.txt.



******************************************************************************/


/**
 *  @file
 *
 *  Implements the Service class.
 *
 *  @see    Service
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/Service.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/uint16_Number.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
Service::Service() {
}


//
//  Destructor.
//
Service::~Service() {
}


//
//  Open a provider stream socket to which consumers may connect.
//
int Service::provider(uint16_t port) {
    return this->provider(port, SOMAXCONN);
}


//
//  Open a provider stream socket to which consumers may connect.
//
int Service::provider(uint16_t port, int backlog) {

    if (backlog > SOMAXCONN) { backlog = SOMAXCONN; }

    struct sockaddr_in sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd >= 0) {
        int rc = this->setReuseAddress(fd, true);
        if (rc != fd) {
            fd = -2;
        } else {
            socklen_t length = sizeof(sa);
            rc = ::bind(fd, reinterpret_cast<struct sockaddr*>(&sa), length);
            if (rc < 0) {
                this->close(fd);
                fd = -3;
            } else {
                rc = ::listen(fd, backlog);
                if (rc < 0) {
                    this->close(fd);
                    fd = -4;
                }
            }
        }
    }

    return fd;
}


//
//  Accept an incoming connection.
//
int Service::accept(int fd) {
    uint32_t address;
    return this->accept(fd, address);
}


//
//  Accept an incoming connection.
//
int Service::accept(int fd, uint32_t& address) {

    struct sockaddr_in sa;
    socklen_t length = sizeof(sa);
    int newfd = ::accept(fd, reinterpret_cast<struct sockaddr*>(&sa), &length);
    if (newfd >= 0) {
        if (length == sizeof(sa))
        {
            address = ntohl(sa.sin_addr.s_addr);
        }
    }
   
    return newfd;
}


//
//  Open a consumer stream socket to a far-end provider.
//
int Service::consumer(uint32_t address, uint16_t port) {

    struct sockaddr_in sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sin_addr.s_addr = htonl(address);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd >= 0) {
        socklen_t length = sizeof(sa);
        int rc = ::connect(fd, reinterpret_cast<struct sockaddr*>(&sa), length);
        if (rc < 0) {
            this->close(fd);
            fd = -2;
        }
    }

    return fd;
}


//
//  Open a peer datagram socket.
//
int Service::peer(uint16_t port) {

    struct sockaddr_in sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (fd >= 0) {
        int rc = this->setReuseAddress(fd, true);
        if (rc != fd) {
            fd = -2;
        } else {
            socklen_t length = sizeof(sa);
            rc = ::bind(fd, reinterpret_cast<struct sockaddr*>(&sa), length);
            if (rc < 0) {
                this->close(fd);
                fd = -3;
            }
        }
    }

    return fd;
}


//
//  Close the socket.
//
int Service::close(int fd, bool abort) {

    if (abort) {
        ::shutdown(fd, SHUT_RDWR);
    }

    // We do the close whether the shutdown works or not.
    // It would risk a resource leak to do otherwise.

    int rc = ::close(fd);
    if (rc >= 0) {
        rc = fd;
    }

    return rc;
}


//
//  Set or clear file controls.
//
int Service::setStatus(int fd, bool enable, long mask) {

    long flags = ::fcntl(fd, F_GETFL, 0);
    if (flags != -1) {
        flags = enable ? (flags | mask) : (flags & (~mask));
        flags = ::fcntl(fd, F_SETFL, flags);
        if (flags != -1) {
            flags = fd;
        }
    }

    return static_cast<int>(flags);
}


//
//  Enable or disable socket options.
//
int Service::setOption(int fd, bool enable, int option) {

    int rc = fd;
    if (rc >= 0) {
        int on = enable ? 1 : 0;
        int rc = ::setsockopt(fd, SOL_SOCKET, option, &on, sizeof(on));
        if (rc >= 0) {
            rc = fd;
        }
    }

    return rc;
}


int Service::setNonBlocking(int fd, bool enable) {
    return this->setStatus(fd, enable, FNDELAY);
}


int Service::setReuseAddress(int fd, bool enable) {
    return this->setOption(fd, enable, SO_REUSEADDR);
}


int Service::setKeepAlive(int fd, bool enable) {
    return this->setOption(fd, enable, SO_KEEPALIVE);
}


int Service::setDebug(int fd, bool enable) {
    return this->setOption(fd, enable, SO_DEBUG);
}


int Service::setLinger(int fd, bool enable) {
    return this->setOption(fd, enable, SO_LINGER);
}


//
//  Map a host name to an internet address.
//
uint32_t Service::address(const char* hostname, size_t index) {

    uint32_t ipaddress = 0;

    // This tries to use the new reentrant gethostbyname_r() which is a GNU
    // extension. It's not available on older 2.4 systems, and maybe not even
    // on older 2.6 systems. If we use the non-reentrant gethostbyname(), there
    // is no point in copying the hostent structure because it contains pointers
    // to character strings which are also in static variables. A deep copy
    // would require a lot of knowledge of the length of those underlying static
    // variables. I'm guessing that's also why the gethostbyname_r() man page
    // has little to say about how large buf needs to be: it depends on what
    // DNS returns and how long the host names are. Nothing is ever simple.
    struct hostent* hostp = 0;
    char* buf = 0;
#if defined(_BSD_SOURCE) || defined(_SVID_SOURCE)
    int rc;
    struct hostent host;
    size_t buflen = 256;
    int error;
 	buf = new char [buflen];
	while (buf != 0) {
    	rc = ::gethostbyname_r(hostname, &host, buf, buflen, &hostp, &error);
    	if (rc == 0) {
    		break;
    	} else if (rc != ERANGE) {
    		hostp = 0;
    		break;
    	} else if (buflen >= 4096) {
    		hostp = 0;
    		break;
    	} else {
    		delete [] buf;
    		buflen *= 2;
    		buf = new char [buflen];
    	}
    }
#else
#	warning Using gethostbyname which is not reentrant!
    hostp = ::gethostbyname(hostname);
#endif
    struct in_addr inaddr;
    if (hostp != 0) {
        if (hostp->h_addrtype == AF_INET) {
            size_t limit;
            for (limit = 0; hostp->h_addr_list[limit] != 0; ++limit) {
                continue;
            }
            if (index < limit) {
                std::memset(&inaddr, 0, sizeof(inaddr));
                size_t size;
                if (hostp->h_length < static_cast<int>(sizeof(inaddr))) {
                    size = hostp->h_length;
                } else {
                    size = sizeof(inaddr);
                }
                std::memcpy(&inaddr, hostp->h_addr_list[index], size);
                ipaddress = ntohl(inaddr.s_addr);
            }
        }
    } else {
        // gethostbyname*() should have done this for us, but we make
        // a list ditch effort anyway.
        int rc = ::inet_aton(hostname, &inaddr);
        if (rc != 0) {
            ipaddress = ntohl(inaddr.s_addr);
        }
    }

    delete [] buf;

    return ipaddress;
}


//
//  Convert address to dot notation.
//
const char* Service::dotnotation(uint32_t address, char* buffer, size_t length) {

    struct in_addr inaddr;
    inaddr.s_addr = htonl(address);
    char* dot = ::inet_ntoa(inaddr);
    ::strncpy(buffer, dot, length);

    return buffer;
}


//
//  Map a service name and protocol name to a port number.
//
uint16_t Service::port(const char* service, const char* protocol) {

    uint16_t port = 0;

    struct servent* portp;
    portp = ::getservbyname(service, protocol);
    if (portp != 0) {
        port = ntohs(portp->s_port);
    } else {
        uint16_t temp;
        size_t length;
        bool successful = uint16_Number(service, temp, length, 64);
        if (successful) {
            port = temp;
        }
    }

    return port;
}


//
//  Show this object on the output object.
//
void Service::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
}


#include "com/diag/desperado/End.h"
