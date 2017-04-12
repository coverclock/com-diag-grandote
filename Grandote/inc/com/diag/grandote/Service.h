#ifndef _COM_DIAG_DESPERADO_SERVICE_H_
#define _COM_DIAG_DESPERADO_SERVICE_H_

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
 *  Declares the Service class.
 *
 *  @see    Service
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a factory that creates sockets for service providers
 *  (server side) or service consumers (client side). If you are using
 *  a framework like ACE, you are better off using its mechanisms for
 *  making socket connections. The purpose of this class is not to
 *  replace those frameworks, but to provide a simple mechanism for
 *  (mostly) testing. Note that internet addresses and port numbers
 *  are passed and returned in host byte order. This has proven easier
 *  to deal with when addresses and ports are printed in binary form.
 *  Addresses and ports are converted to network byte order by the
 *  methods which take them as arguments.
 *
 *  This class does not (yet) support IPV6 addresses because I (at
 *  the moment) have no way to unit test them.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Service : public Object {

public:

    /**
     *  Constructor.
     */
    explicit Service();

    /**
     *  Destructor.
     */
    virtual ~Service();

    /**
     *  Create a stream socket and bind it to the specified port number on the
     *  local host. Socket option SO_REUSEADDR is set automatically. The queue
     *  depth for incoming connection requests from consumers is set to the
     *  platform maximum (SOMAXCONN), a typical value for which is 128.
     *
     *  @param port     is a port number in host byte order.
     *
     *  @return a socket or a negative number if error.
     */
    virtual int provider(uint16_t port);

    /**
     *  Create a stream socket and bind it to the specified port number on the
     *  local host. Socket option SO_REUSEADDR is set automatically. Pending
     *  incoming connection requests from consumers are limited to the
     *  specified backlog; connection requests after this queue depth is
     *  reached will be refused.
     *
     *  @param port     is a port number in host byte order.
     *
     *  @param backlog  is the maximum queue depth for connection requests.
     *
     *  @return a socket or a negative number if error.
     */
    virtual int provider(uint16_t port, int backlog);

    /**
     *  Given a service provider port, wait until a connection request
     *  arrives and return a new socket connected to the far-end service
     *  consumer.
     *
     *  @param fd       is a service provider socket.
     *
     *  @return a socket or a negative number if error.
     */
    virtual int accept(int fd);

    /**
     *  Given a service provider port, wait until a connection request
     *  arrives and return a new socket connected to the far-end service
     *  consumer.
     *
     *  @param fd       is a service provider socket.
     *
     *  @param address  if successful will be filled in with the address
     *                  of the far-end service consumer.
     *
     *  @return a socket or a negative number if error.
     */
    virtual int accept(int fd, uint32_t& address);

    /**
     *  Create a stream socket to the specified port on the host identified by
     *  the specified IPV4 internet address.
     *
     *  @param address  is a thirty-two-bit IPV4 internet address in host byte
     *                  order.
     *
     *  @param port     is a port number in host byte order.
     *
     *  @return a socket or a negative number if error.
     */
    virtual int consumer(uint32_t address, uint16_t port);

    /**
     *  Create a datagram socket configured for connectionless datagrams.
     *
     *  @param port     is a port number in host byte order.
     *
     *  @return a socket or a negative number if error.
     */
    virtual int peer(uint16_t port);

    /**
     *  Close the socket.
     *
     *  @param fd       is the socket.
     *
     *  @param abort    if true, cause a TCP ABORT do be done, possibly
     *                  losing pending data in any in-core buffers (not
     *                  recommended).
     *
     *  @return the closed socket or a negative number if error.
     */
    virtual int close(int fd, bool abort = false);

    /**
     *  Set or clear the specified file status bits.
     *
     *  @param fd       is the socket.
     *
     *  @param enable   if true sets the bits, otherwise clears them.
     *
     *  @param mask     is the file control bit mask.
     *
     *  @return the socket if successful, a negative number otherwise.
     */
    virtual int setStatus(int fd, bool enable, long mask);

    /**
     *  Set or clear the file control option FNDELAY.
     *
     *  @param fd       is the socket.
     *
     *  @param enable   if true enables the option, otherwise disables it.
     *
     *  @return the socket if successful, a negative number otherwise.
     */
    virtual int setNonBlocking(int fd, bool enable = true);

    /**
     *  Set or clear the specified socket option.
     *
     *  @param fd       is the socket.
     *
     *  @param enable   if true enables the option, otherwise disables it.
     *
     *  @param option   is the socket option.
     *
     *  @return the socket if successful, a negative number otherwise.
     */
    virtual int setOption(int fd, bool enable, int option);

    /**
     *  Set or clear the socket option SO_REUSEADDR.
     *
     *  @param fd       is the socket.
     *
     *  @param enable   if true enables the option, otherwise disables it.
     *
     *  @return the socket if successful, a negative number otherwise.
     */
    virtual int setReuseAddress(int fd, bool enable = true);

    /**
     *  Set or clear the socket option SO_KEEPALIVE.
     *
     *  @param fd       is the socket.
     *
     *  @param enable   if true enables the option, otherwise disables it.
     *
     *  @return the socket if successful, a negative number otherwise.
     */
    virtual int setKeepAlive(int fd, bool enable = true);

    /**
     *  Set or clear the socket option SO_LINGER.
     *
     *  @param fd       is the socket.
     *
     *  @param enable   if true enables the option, otherwise disables it.
     *
     *  @return the socket if successful, a negative number otherwise.
     */
    virtual int setLinger(int fd, bool enable = true);

    /**
     *  Set or clear the socket option SO_LINGER.
     *
     *  @param fd       is the socket.
     *
     *  @param enable   if true enables the option, otherwise disables it.
     *
     *  @return the socket if successful, a negative number otherwise.
     */
    virtual int setDebug(int fd, bool enable = true);

    /**
     *  Try to convert the specified host name into a internet address.
     *  The host name may be a domain name or an IPV4 internet address
     *  in dot notation. If multiple internet addresses are identified
     *  by the host name, the index selects the internet address to
     *  return.
     *
     *  @param hostname is a host name or an IPV4 internet address in dot
     *                  notation.
     *
     *  @param  index   is an index which may optionally select one of
     *                  several internet addresses.
     *
     *  @return a thirty-two-bit IPV4 address or zero if none.
     */
    virtual uint32_t address(const char* hostname, size_t index = 0);

    /**
     *  Convert an internet address into printable form in dot notation.
     *
     *  @param address  is an IPV4 internet address in host byte order.
     *
     *  @param buffer   is a buffer into which the string is placed.
     *
     *  @param length   is the length of the buffer in octets.
     *
     *  @return a pointer to buffer.
     */
    virtual const char* dotnotation(
        uint32_t address,
        char* buffer,
        size_t length
    );

    /**
     *  Return a port number identified by a service name and a protocol
     *  name. The service name may be a string naming a service or it may
     *  be a numeric string. If the protocol name is null, any protocol may
     *  be chosen, although "tcp" is the default.
     *
     *  @param service  is a service name or a numeric string.
     *
     *  @param protocol is a protocol name, or null if any protocol may be
     *                  used. If you do not know which protocol to use,
     *                  take the default. The Desperado Input/Output framework
     *                  only supports the TCP protocol, although you
     *                  can use the your platform's native I/O interface
     *                  to use others.
     *
     *  @return a sixteen-bit port number or zero if none.
     */
    virtual uint16_t port(const char* service, const char* protocol = "tcp");

    /**
     *  Displays internal information about this object to the specified
     *  output object. Useful for debugging and troubleshooting.
     *
     *  @param  level   sets the verbosity of the output. What this means
     *                  is object dependent. However, the level is passed
     *                  from outer to inner objects this object calls the
     *                  show methods of its inherited or composited objects.
     *
     *  @param  display points to the output object to which output is
     *                  sent. If null (zero), the default platform output
     *                  object is used as the effective output object. The
     *                  effective output object is passed from outer to
     *                  inner objects as this object calls the show methods
     *                  of its inherited and composited objects.
     *
     *  @param  indent  specifies the level of indentation. One more than
     *                  this value is passed from outer to inner objects
     *                  as this object calls the show methods of its
     *                  inherited and composited objects.
     */
    virtual void show(int level = 0, Output* display = 0, int indent = 0) const;
};

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Service unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestService(void);
#endif


#endif
