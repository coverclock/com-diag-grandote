# com-diag-grandote

COPYRIGHT

Copyright 2017 Digital Aggregates Corporation, Arvada Colorado USA.

LICENSE

Licensed under the terms of FSF LGPL v2.1.

ABSTRACT

This is a work in progress.

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
written. That target - laughably small in light of today's 64-bit
gigaHertz microprocessors (and the reason for Diminuto's name) - is
long gone, but the Diminuto library remains and continues to evolve.
I don't know that anyone but me has ever used Desperado, but portions
of Diminuto are shipping in a number of commercial products from several
of my clients.

The pthread portions of Desperadito - and, hence, Grandote - were forked
from Hayloft, a C++ framework for using Amazon Web Service (AWS) Simple
Storage Service (s3).

The unit tests are a mixture of straight C++ programs (whose
implementations were a little strange to make them easier to use in
VxWorks) from the ancient Desperado code base, and Google Test programs
from the Hayloft code base. The Google Test programs use a thin helper
layer from yet another project, Lariat.

To use Grandote you need to also install Diminuto, Lariat, and Google Test
(but NOT Desperadito, Desperado, or Hayloft).

I have written tens if not hundreds of thousands of lines of C++
for embedded products, mostly during my years at Bell Labs and its
various spinoffs. At one time I was a strong advocate for the use of
C++ in embedded products. But over the years, I have seen a decline in
the use of C++ for such work, and an increase in the use of C for the
systems layer, and languages like Python, Java, and even JavaScript
in the application layer. I can't argue with the economics of that:
you can hire less expensive developers for those other langauges. As
a consequence of dealing with mixed language product development,
I've done my share of coding in Python and Java, and have even hacked
JavaScript. Meanwhile, C++ has evolved into a language so complex that
my clients can't afford - or can't find - developers with that skill set
and that level of expertise.

But perhaps my interest in Grandote means I'm not ready to give up on C++
quite yet.

REPOSITORIES

<https://github.com/coverclock/com-diag-grandote>

<https://github.com/coverclock/com-diag-diminuto>

<https://github.com/coverclock/com-diag-lariat>

<https://github.com/google/googletest>

<https://github.com/coverclock/com-diag-desperadito>

<https://github.com/coverclock/com-diag-desperado>

<https://github.com/coverclock/com-diag-hayloft>

CONTACT

Chip Overclock<br/>
<mailto:coverclock@diag.com><br/>
Digital Aggregates Corporation<br/>
3440 Youngfield Street #209<br/>
Wheat Ridge CO 80033<br/>
USA<br/>

NOTES

    cd ~/src
    git clone https://github.com/google/googletest
    cd googletest
    cmake -Dgtest_build_samples=ON -Dgtest_build_tests=ON `pwd`
    make
    make test
