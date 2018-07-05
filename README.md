ubertk
======

![banner](http://nuclear.mutantstargoat.com/sw/ubertk/img/utk_banner.png)
(screenshots: https://imgur.com/a/04qI6kT)

About
-----
Ubertk is an API-agnostic C++ widget toolkit for graphics programs. The
user program supplies a set of drawing callbacks required by ubertk, and
calls a function to notify ubertk about low level mouse and keyboard
events.

The set of drawing operations which must be defined by the user are:
  * set current color (RGBA) for future drawing operations.
  * set clip rectangle for future drawing operations.
  * draw image at a certain position, given the image pixels.
  * draw rectangle.
  * draw line of a certain width.
  * draw a text string using a certain font size.
  * query the width of a text string at a certain font size.
  * query the amount of spacing to leave between rows of text.

A sample implementation of these callbacks for OpenGL, using freetype
for text rendering is provided in the test program, which resides in the
project root directory. Note that the dependency to OpenGL, GLUT and
freetype are only for the test program. Everything in the `src`
directory, which is the only part necessary to build and use the library
in your own programs is plain ISO C++ 98 without any dependencies to 3rd
party libraries other than the standard C++ library.

For another example of OpenGL callbacks take a look at the rototool
project: https://github.com/MutantStargoat/rototool

License
-------
Copyright (C) 2007 - 2018 John Tsiombikas, Michael Georgoulopoulos,
    and Kostas Michalopoulos.

Ubertk is free software. Feel free to use, modify, and/or redistribute
it under the terms of the 3-clause BSD license. See COPYING for details.

What this means in plain terms is that you can use ubertk any way you
like, as long as you give attribution to the authors, retain all
relevant copyright notices, and not claim to have our endorsement for
whatever you're doing with it.

Build
-----
To build and install `libutk` on UNIX, run the usual:

    ./configure
    make
    make install

See `./configure --help` for build-time options. 

If you wish to build the test program, run:

    make test

To build on windows, you may use the included visual studio project, or use
mingw, in which case just follow the UNIX instructions above.

To cross-compile for windows with mingw-w64, try the following incantation:

    ./configure --prefix=/usr/i686-w64-mingw32
    make CC=i686-w64-mingw32-gcc CXX=i686-w64-mingw32-g++ AR=i686-w64-mingw32-ar sys=mingw
    make install sys=mingw


Contact
-------
Curent maintainer ubertk is John Tsiombikas <nuclear@member.fsf.org>.

Only properly formatted (hard wrapped at 72 columns, no top-posting)
text emails will be accepted. You may use multi-part text+html, but I
will ignore the html part, so make sure the text version contains all
the necessary information.
