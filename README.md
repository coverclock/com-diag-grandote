# com-diag-grandote

COPYRIGHT

Copyright 2017 Digital Aggregates Corporation, Arvada Colorado USA.

LICENSE

Licensed under the terms of FSF LGPL v2.1.

ABSTRACT

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

I have written tens if not hundreds of thousands of lines of C++
for embedded products, mostly during my years at Bell Labs and its
various spinoffs, under a variety of operating systems including Linux
and VxWorks. At one time I was a strong advocate for the use of C++ in
embedded products. But over the years, I have seen a decline in the use
of C++ for such work, and an increase in the use of C for the systems
layer, and languages like Python, Java, and even JavaScript for the
application layer. I can't argue with the economics of that: you can
hire less expensive developers for those other languages, and writing
in those languages is more productive than in C++. As a consequence of
dealing with mixed language product development, I've done my share of
coding in Python and Java, and have even hacked JavaScript. Meanwhile, C++
has evolved into a language so complex that my clients can't afford - or
can't find - developers with that skill set and that level of expertise.

But perhaps my interest in Grandote means I'm not ready to give up on C++
quite yet.

REPOSITORIES

<https://github.com/coverclock/com-diag-grandote>

<https://github.com/coverclock/com-diag-diminuto>

<https://github.com/coverclock/com-diag-lariat>

<https://github.com/google/googletest>

<https://github.com/coverclock/com-diag-desperadito>

<https://github.com/coverclock/com-diag-hayloft>

<https://github.com/coverclock/com-diag-desperado>

CONTACT

Chip Overclock<br/>
<mailto:coverclock@diag.com><br/>
Digital Aggregates Corporation<br/>
3440 Youngfield Street #209<br/>
Wheat Ridge CO 80033<br/>
USA<br/>
<http://www.diag.com><br/>

BUILD

    cd ~/src
    git clone https://github.com/google/googletest
    ( cd googletest; cmake -Dgtest_build_samples=ON -Dgtest_build_tests=ON `pwd`; make; make test )
    git clone https://github.com/coverclock/com-diag-diminuto
    make -C com-diag-diminuto/Diminuto pristine all
    git clone https://github.com/coverclock/com-diag-lariat
    make -C com-diag-lariat/Lariat pristine all
    git clone https://github.com/coverclock/com-diag-grandote
    make -C com-diag-grandote/Grandote pristine all
