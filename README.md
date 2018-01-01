# com-diag-grandote

Copyright 2005-2018 Digital Aggregates Corporation, Arvada Colorado USA.

## License

Licensed under the terms of FSF LGPL v2.1.

## Abstract

Grandote (Spanish for "ginormous") is a C++ layer on top of the Diminuto
(Spanish for "tiny") library.  Diminuto is a C-based library specifically
written for Linux/GNU. Grandote is fork of Desperadito, which itself
is a fork of Desperado. Desperado is a C++ library that was intended
to abstract out the underlying operating system so that applications
written on top of it would be portable to other hardware targets and
software platforms like Linux, Cygwin, VxWorks, etc. In this incarnation,
Grandote provides C++ facilities on top of Diminuto's OS abstraction.

To make this even more complicated, Diminuto itself was at one time an
OS choice for Desperado, which referred not to the Diminuto library, but
to the ARMv4-based hardware target for which Diminuto was originally
written. That target, laughably small in light of today's 64-bit
gigaHertz multicore microprocessors (and the reason for Diminuto's name),
is long gone, but the Diminuto library remains and continues to evolve.
I don't know that anyone but me has ever used Desperado, but portions
of Diminuto are shipping in a number of commercial products from several
of my clients.

The POSIX thread portions of Desperadito - and, hence, Grandote - were
forked from Hayloft, a C++ framework for using Amazon Web Service (AWS)
Simple Storage Service (S3). A tiny portion of Diminuto was in C++,
and those facilities have been moved into Grandote, making Diminuto
purely C. The unit tests are a mixture of straight C++ programs (whose
implementations were a little strange to make them easier to use in
VxWorks) from the ancient Desperado code base, Google Test programs from
the Hayloft code base, and even some unit test main programs from the
Diminuto code base that use the Diminuto unit test framework. The Google
Test programs use a thin helper layer from yet another project, Lariat.
For new unit tests, I strongly recommend Google Test, although the
Diminuto unit test framework is simple and easy to use.

To use Grandote you need to also install Diminuto, Lariat, and Google
Test. This is easier than it sounds. I had issues getting the unittest
main program that uses Google Test to link under Ubuntu 14.04 and the
problem defied my ability to fix it; I ran the same build on Ubuntu 16.04
and had one tiny dependency issue that was easily remedied; my guess is
there was really only that one small issue, and the other error messages
that side-tracked me were spurious.

Grandote, Desperado, Desperadito, Hayloft, Lariat, and Diminuto were all
written by me, are the intellectual property of the Digital Aggregates
Corporation, and are licensed using one FSF GNU license or another.

## Remarks

I started working on Desperado, the core from which Grandote was derived,
in 2005. The code has transitioned through a number of source code
control systems, as they became available and popular: RCS, CVS, SVN,
and finally GIT. There were a number of reasons for my interested in
Desperado.

For years I had done development in C++ for embedded systems using the
VxWorks real-time operating system (RTOS). I could see that while the
platform for many new products was going to transition to Linux, there
would be a substantial revenue-generating base of legacy products still
based on VxWorks. I wanted to experiment with abstracting the platform
dependencies out from the C++ applications so that those applications
could be run on either VxWorks and Linux.

I had had a lot of experience integrating C++ systems into existing C
systems, or vice versa, and wanted to establish a mechanism for C code
to take advantage of C++ frameworks.

I saw a need for a way to abstract data sources and sinks into objects
with a common interface that could be used via dependency injection to
tie software components together.

Despite other's skepticism around the use of C++ templates in resource
constrained embedded systems, I believed templates could be used as a
kind of type-safety wrapper, and also to judiciously generate code for
hardware platforms whose underlying integer types were of different sizes.

I have had a long time interest in timekeeping and calendar systems,
a theme that shows up in many of my projects. Desperado gave me a way
to explore this in detail.

There are several frameworks - Boost comes to mind most readily - that
package most of the complexity of using POSIX Threads into a more easily
digestible form. But I have learned from experience working on large
product development teams that it is all to easy to use such frameworks
and not understand how they work and find yourself unable to debug your
own code when wackiness ensues. So developing my own simple Thread
framework was my way of learning how POSIX Threads work under the hood.

I explored many of these same themes in Diminuto, my C-based Linux systems
programming library. My VxWorks days are - as far as I can tell anyway -
long gone. I consider most of my platform-portability work in Grandote
obsolete and for the most part no longer worry much about it. But many
of the other ideas I explored in Desperado are of continuing value and
I maintain them in Grandote.

I have written tens if not hundreds of thousands of lines of C++ for
embedded products, mostly during my years at Bell Labs and its various
spinoffs.  At one time I was a strong advocate for the use of C++ in
embedded products. But over the years, I have seen a decline in the use
of C++ for such work, and an increase in the use of C for the systems
layer, and languages like Python, Java, and even JavaScript for the
application layer.

I can't argue with the economics of that: you can hire less expensive
developers for those other languages, and writing in those languages
is more productive than in C++. As a consequence of dealing with mixed
language product development, I've done my share of coding in Python
and Java, and have even hacked JavaScript.

Meanwhile, C++ has evolved into a language so complex that my clients
can't afford - or can't find - developers with that skill set and that
level of expertise. I myself need to come up to speed with some of the
more recent C++ features, and determine how I might leverage them in
the kind of work that I do.. Perhaps my interest in Grandote means I'm
not ready to give up on C++ quite yet.

## Repositories

<https://github.com/coverclock/com-diag-grandote>

<https://github.com/coverclock/com-diag-diminuto>

<https://github.com/coverclock/com-diag-lariat>

<https://github.com/google/googletest>

<https://github.com/coverclock/com-diag-desperadito>

<https://github.com/coverclock/com-diag-hayloft>

<https://github.com/coverclock/com-diag-desperado>

## Contact

Chip Overclock<br/>
<mailto:coverclock@diag.com><br/>
Digital Aggregates Corporation<br/>
3440 Youngfield Street #209<br/>
Wheat Ridge CO 80033<br/>
USA<br/>
<http://www.diag.com><br/>

## Build

    cd ~/src
    git clone https://github.com/google/googletest
    ( cd googletest; mkdir out; cd out; cmake -Dgtest_build_samples=ON -Dgtest_build_tests=ON ..; make; make test )
    git clone https://github.com/coverclock/com-diag-diminuto
    ( cd com-diag-diminuto/Diminuto; make pristine depend all )
    git clone https://github.com/coverclock/com-diag-lariat
    ( cd com-diag-lariat/Lariat; make pristine depend all )
    git clone https://github.com/coverclock/com-diag-grandote
    ( cd com-diag-grandote/Grandote; make pristine depend all )

