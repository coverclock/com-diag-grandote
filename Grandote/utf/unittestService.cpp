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
 *  Implements the Service unit test.
 *
 *  @see    Service
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */

#include <unistd.h>
#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/Service.h"
#include "com/diag/desperado/Service.h"
#include "com/diag/desperado/StreamSocket.h"
#include "com/diag/desperado/StreamSocket.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/FileInput.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/FileOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Desperado.h"

static Service static_Service;

CXXCAPI int unittestService(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;
    int rc;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::static_Service.show();

    Service service;

    service.show();

    printf("%s[%d]: addresses\n", __FILE__, __LINE__);

    uint32_t address;

    address = service.address("127.0.0.1");
    if (address != 0x7f000001) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, address, 0x7f000001);
        ++errors;
    }

    address = service.address("localhost");
    if (address != 0x7f000001) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, address, 0x7f000001);
        ++errors;
    }

    address = service.address("www.diag.com");
    if (address == 0) {
        errorf("%s[%d]: (0x%x==0x%x)!\n",
            __FILE__, __LINE__, address, 0);
        ++errors;
    }
    char buffer[64];
    printf("www.diag.com=0x%08x=%s\n",
        address, service.dotnotation(address, buffer, sizeof(buffer)));

    // This next test should fail, and typically does. But I recently had
    // DNS return "208.68.143.50" a.k.a. 0xd0448f32 to this query. My guess is
    // it is ComCast (or whatever they call themselves these days) trying to be
    // oh so helpful. Bastards. So I don't increment the error count if that
    // happens.
    address = service.address("invalid.domain");
    if (address != 0) {
        errorf("%s[%d]: ((0x%x=%s)!=0x%x)!\n",
            __FILE__, __LINE__, address, service.dotnotation(address, buffer, sizeof(buffer)), 0);
    }

    printf("%s[%d]: ports\n", __FILE__, __LINE__);

    uint16_t port;

    port = service.port("80");
    if (port != 80) {
        errorf("%s[%d]: (%u==%u)!\n",
            __FILE__, __LINE__, port, 80);
        ++errors;
    }

    port = service.port("80", 0);
    if (port != 80) {
        errorf("%s[%d]: (%u==%u)!\n",
            __FILE__, __LINE__, port, 80);
        ++errors;
    }

    port = service.port("80", "tcp");
    if (port != 80) {
        errorf("%s[%d]: (%u==%u)!\n",
            __FILE__, __LINE__, port, 80);
        ++errors;
    }

    port = service.port("80", "udp");
    if (port != 80) {
        errorf("%s[%d]: (%u==%u)!\n",
            __FILE__, __LINE__, port, 80);
        ++errors;
    }

    port = service.port("http");
    if (port != 80) {
        errorf("%s[%d]: (%u==%u)!\n",
            __FILE__, __LINE__, port, 80);
        ++errors;
    }

    port = service.port("http", 0);
    if (port != 80) {
        errorf("%s[%d]: (%u==%u)!\n",
            __FILE__, __LINE__, port, 80);
        ++errors;
    }

    port = service.port("http", "tcp");
    if (port != 80) {
        errorf("%s[%d]: (%u==%u)!\n",
            __FILE__, __LINE__, port, 80);
        ++errors;
    }

    port = service.port("tftp", "udp");
    if (port != 69) {
        errorf("%s[%d]: (%u==%u)!\n",
            __FILE__, __LINE__, port, 69);
        ++errors;
    }

    port = service.port("login");
    if (port != 513) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 513);
        ++errors;
    }

    port = service.port("login", 0);
    if (port != 513) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 513);
        ++errors;
    }

    port = service.port("login", "tcp");
    if (port != 513) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 513);
        ++errors;
    }

    port = service.port("login", "udp");
    if (port != 0) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 0);
        ++errors;
    }

    port = service.port("who");
    if (port != 0) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 0);
        ++errors;
    }

    port = service.port("who", 0);
    if (port != 513) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 513);
        ++errors;
    }

    port = service.port("who", "tcp");
    if (port != 0) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 0);
        ++errors;
    }

    port = service.port("who", "udp");
    if (port != 513) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 513);
        ++errors;
    }

    port = service.port("coverclock");
    if (port != 0) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 0);
        ++errors;
    }

    port = service.port("coverclock", 0);
    if (port != 0) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 0);
        ++errors;
    }

    port = service.port("cloverclock", "tcp");
    if (port != 0) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 0);
        ++errors;
    }

    port = service.port("coverclock", "udp");
    if (port != 0) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, port, 0);
        ++errors;
    }

    printf("%s[%d]: peer\n", __FILE__, __LINE__);

    int fd;

    uint16_t udpport = static_cast<uint16_t>(65535);
    fd = service.peer(udpport);
    if (fd < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, fd, 0, errno);
        ++errors;
    }
    rc = service.close(fd);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    printf("%s[%d]: consumer\n", __FILE__, __LINE__);

    fd = service.consumer(service.address("www.diag.com"), service.port("http"));
    if (fd < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, fd, 0, errno);
        ++errors;
    }
    rc = service.close(fd);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    fd = service.consumer(service.address("www.amazon.com"), service.port("http"));
    if (fd < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, fd, 0, errno);
        ++errors;
    }
    rc = service.close(fd);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    printf("%s[%d]: provider\n", __FILE__, __LINE__);

    fd = service.provider(0xfff0);
    if (fd < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, fd, 0, errno);
        ++errors;
    }
    rc = service.close(fd);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    printf("%s[%d]: options\n", __FILE__, __LINE__);

    fd = service.provider(0xfff0);
    if (fd < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, fd, 0, errno);
        ++errors;
    }

    rc = service.setNonBlocking(fd, true);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setNonBlocking(fd, false);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setReuseAddress(fd, true);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setReuseAddress(fd, false);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setKeepAlive(fd, true);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setKeepAlive(fd, false);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setDebug(fd, true);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setDebug(fd, false);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setLinger(fd, true);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.setLinger(fd, false);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    rc = service.close(fd);
    if (rc != fd) {
        errorf("%s[%d]: (%d!=%d) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    printf("%s[%d]: producer-consumer\n", __FILE__, __LINE__);

    int rendezvous = service.provider(0xfff0);
    if (rendezvous < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, rendezvous, 0, errno);
        ++errors;
    }

    int consumer = service.consumer(service.address("localhost"), 0xfff0);
    if (consumer < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, consumer, 0, errno);
        ++errors;
    }

    address = 0;
    int producer = service.accept(rendezvous, address);
    if (producer < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, producer, 0, errno);
        ++errors;
    }
    printf("consumer=0x%08x=%s\n",
        address, service.dotnotation(address, buffer, sizeof(buffer)));
    if (address != 0x7f000001) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, address, 0x7f000001);
        ++errors;
    }

    // This only works because the kernel buffers socket data.

    for (int index = 0; index < 256; ++index) {

        char sent = index;
        char received = ~sent;

        rc = ::write(producer, &sent, 1);
        if (rc != 1) {
            errorf("%s[%d]: (%u!=%u) (%d)!\n",
                __FILE__, __LINE__, rc, 1, errno);
            ++errors;
        }

        rc = ::read(consumer, &received, 1);
        if (rc != 1) {
            errorf("%s[%d]: (%u!=%u) (%d)!\n",
                __FILE__, __LINE__, rc, 1, errno);
            ++errors;
        }
        if (received != sent) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, received, sent);
            ++errors;
        }

        sent = index;
        received = ~sent;

        rc = ::write(consumer,  &sent, 1);
        if (rc != 1) {
            errorf("%s[%d]: (%u!=%u) (%d)!\n",
                __FILE__, __LINE__, rc, 1, errno);
            ++errors;
        }

        rc = ::read(producer, &received, 1);
        if (rc != 1) {
            errorf("%s[%d]: (%u!=%u) (%d)!\n",
                __FILE__, __LINE__, rc, 1, errno);
            ++errors;
        }
        if (received != sent) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, received, sent);
            ++errors;
        }
    }

    rc = service.close(producer);
    if (rc != producer) {
        errorf("%s[%d]: (%u!=%u) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }
    rc = service.close(consumer);
    if (rc != consumer) {
        errorf("%s[%d]: (%u!=%u) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }
    rc = service.close(rendezvous);
    if (rc != rendezvous) {
        errorf("%s[%d]: (%u!=%u) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    printf("%s[%d]: StreamSockets\n", __FILE__, __LINE__);

    rendezvous = service.provider(0xfff0);
    if (rendezvous < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, rendezvous, 0, errno);
        ++errors;
    }

    consumer = service.consumer(service.address("localhost"), 0xfff0);
    if (consumer < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, consumer, 0, errno);
        ++errors;
    }

    producer = service.accept(rendezvous);
    if (producer < 0) {
        errorf("%s[%d]: (%u<%u) (%d)!\n",
            __FILE__, __LINE__, producer, 0, errno);
        ++errors;
    }

    StreamSocket nearend(consumer);
    nearend.show();
    Input& nearendin = nearend.input();
    nearendin.show();
    Output& nearendout = nearend.output();
    nearendout.show();

    StreamSocket farend(producer);
    farend.show();
    Input& farendin = farend.input();
    farendin.show();
    Output& farendout = farend.output();
    farendout.show();

    // This only works because the kernel buffers socket data.

    for (int index = 0; index < 256; ++index) {

        char sent = index;
        char received = ~sent;

        rc = farendout(sent);
        if (rc == EOF) {
            errorf("%s[%d]: (%u==%u) (%d)!\n",
                __FILE__, __LINE__, rc, EOF, errno);
            ++errors;
        }

        received = nearendin();
        if (received != sent) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, received, sent);
            ++errors;
        }

        sent = index;
        received = ~sent;

        rc = nearendout(sent);
        if (rc == EOF) {
            errorf("%s[%d]: (%u==%u) (%d)!\n",
                __FILE__, __LINE__, rc, EOF, errno);
            ++errors;
        }

        received = farendin();
        if (received != sent) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, received, sent);
            ++errors;
        }
    }

    rc = service.close(producer);
    if (rc != producer) {
        errorf("%s[%d]: (%u!=%u) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }
    rc = service.close(consumer);
    if (rc != consumer) {
        errorf("%s[%d]: (%u!=%u) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }
    rc = service.close(rendezvous);
    if (rc != rendezvous) {
        errorf("%s[%d]: (%u!=%u) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    printf("%s[%d]: Files\n", __FILE__, __LINE__);

    rendezvous = service.provider(0xfff0);
    if (rendezvous < 0 ) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, rendezvous, 0, errno);
        ++errors;
    }

    consumer = service.consumer(service.address("localhost"), 0xfff0);
    if (consumer < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, consumer, 0, errno);
        ++errors;
    }

    FILE* nearendfile = ::fdopen(consumer, "r+");
    if (nearendfile == 0) {
        errorf("%s[%d]: (0x%x==0x%x) (%d)!\n",
            __FILE__, __LINE__, nearendfile, 0, errno);
        ++errors;
    }

    producer = service.accept(rendezvous);
    if (producer < 0) {
        errorf("%s[%d]: (%d<%d) (%d)!\n",
            __FILE__, __LINE__, producer, 0, errno);
        ++errors;
    }

    FILE* farendfile = ::fdopen(producer, "r+");
    if (farendfile == 0) {
        errorf("%s[%d]: (0x%x==0x%x) (%d)!\n",
            __FILE__, __LINE__, farendfile, 0, errno);
        ++errors;
    }

    FileInput nearendfileinput(nearendfile);
    nearendfileinput.show();
    FileOutput nearendfileoutput(nearendfile);
    nearendfileoutput.show();

    FileInput farendfileinput(farendfile);
    farendfileinput.show();
    FileOutput farendfileoutput(farendfile);
    farendfileoutput.show();

    // This only works because the kernel buffers socket data.

    for (int index = 0; index < 256; ++index) {

        char sent = index;
        char received = ~sent;

        rc = farendfileoutput(sent);
        if (rc == EOF) {
            errorf("%s[%d]: (%u==%u) (%d)!\n",
                __FILE__, __LINE__, rc, EOF, errno);
            ++errors;
        }
        farendfileoutput();

        received = nearendfileinput();
        if (received != sent) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, received, sent);
            ++errors;
        }

        sent = index;
        received = ~sent;

        rc = nearendfileoutput(sent);
        if (rc == EOF) {
            errorf("%s[%d]: (%u==%u) (%d)!\n",
                __FILE__, __LINE__, rc, EOF, errno);
            ++errors;
        }
        nearendfileoutput();

        received = farendfileinput();
        if (received != sent) {
            errorf("%s[%d]: (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, received, sent);
            ++errors;
        }
    }

    std::fclose(nearendfile);
    std::fclose(farendfile);

    rc = service.close(rendezvous);
    if (rc != rendezvous) {
        errorf("%s[%d]: (%u!=%u) (%d)!\n",
            __FILE__, __LINE__, rc, fd, errno);
        ++errors;
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
