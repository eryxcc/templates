# templates
My templates for creating short programs. Mostly for programming contests,
but they have other uses too. This uses mainly C++, and is written mainly for Linux, but
should work in other operating systems, possibly with some work.

### Credentials

See [my achievements in programming contests](https://www.mimuw.edu.pl/~erykk/xe-contest.php).
Many of these achievements have been obtained using various parts of this file.

## Important components

### topcoder.h

This file defines macros and functions that allow one to write C++ programs quickly.
It is called topcoder.h because it was originally intended for TopCoder, but it is
useful for basically everything.
It can be #included, which in turn does all the STL includes; but it also has special
comments which work as a special markdown for use by templator (described next).
It contains:

* basic macros and includes such as FOR
* quantifier macros
* parsers
* memoizer macros
* 2D geometry, based on complex numbers
* a function to add guards to a table
* fll type for exact computations on fractions
* modar type for modulo computations
* BINFIRST to find the first value where a condition is satisfied using binary search
* matrix type
* removeStackLimit
* a powerful logger 
 
### templator

This tool parses the C++ given on the input, and replaces `#include <topcoder.h>` with
only the parts of `topcoder.h` which are actually relevant. This is done by simple
search text: for example, if your file contains the substring `cld`, it is assumed that
you are using the `cld` type from `topcoder.h`, and everything that you need from
topcoder.h to use the `cld` type is included (that is, inclusion of `complex.h`,
definition of `long double` as `ld`, and definition of `complex<long double>` as `cld`).

This has two purposes:

* It helps with the UCR (Useless Code Rule) on TopCoder.

* The code is nicer, and it compiles faster because there are no unused STL includes.

### thread engine

This tool helps with contests where you run the tests on your local machine, such as
Google Code Jam or Facebook Hacker Cup. Typically, in these contests there is a single
test file that contains, say, 100 cases. Thread engine works by assigning these cases
to separate processes, thus making use of all your cores.

To use threadengine, you need to put the comment `\\ proceed` in your `solveCase()`
function in `work.cpp`, just after the test case has been read. This will be replaced
by the call to `checkCase()` in threadengine, which will decide whether to fork, to run the case on
the "main" process, or to wait for the child processes to complete. The comment
`\ finish` will call `finish()`, which aggregates all the answers and outputs them.
The comment `\\ Eryx` is replaced with `#include <threadengine.cpp>`.

The replacements are done by the Makefile (`make threaded`).

### graph.h

This file is to be included for programs with graphical outputs. It is a convenient
tool for making pictures using C++. Pictures can be either presented on the screen
(using libsdl version 1.x), saved as a PNG file (using libgd), or saved as a SVG file
(not using any library). This was intended to help with two types of contests: (1) 
graphical design contests at TopCoder studio, (2) programming marathons such as
Deadline24, Marathon24, or Challenge24. Of course it is helpful whenever you want to
draw something.

## Directory structure

### libs

This contains all the header files. They may be also directly included into the
programming contest solutions if necessary.

### newio

This contains templates for programming contests where input/output are done with
simple text files. The same template is used both for contests where an online judge
executes your programs, and for contests where you run the tests on your own computer.
Templator and threadengine are both used. Run `make-contest.sh` to create a copy
for a new contest round.

### sample

This contains templates for 24 hour programming marathons. There are lots of problem
types in Challenge24 (requiring display or not, interactivity or not, audio or not,
networking or not, etc.) and there are samples for each of them. For Deadline24 only 
`dead.cpp` is important; login data and the contest type (Deadline24/Marathon24) is
configured in `login.h` in libs.

### topcoder

This contains tools for TopCoder. It is poorly documented, and it needs lots of
configuration to actually be usable, but I include it anyway.

Assumed to work with the old Java arena, with the
following plugins installed: CodeProcessor, FileEdit, and a testing plugin (TZTester
or moj). The file template2.txt contains my template for FileEdit; it creates
`PROBLEM.cpp`, which is to be processed by the `extractor` tool into `PROBLEM.cc`
(just run the `ext` script). Write your solution in `PROBLEM.cc`, and then run `run`,
which uses the templator tool to transform it back into `PROBLEM.cpp` which could be
submitted to TopCoder; the program is also compiled and ran on the sample test cases.
The file cacher.cc is included as a test of the macros in topcoder.h, and as a way
to cache your compiler.

### other

This directory contains some extra pre-written code, and useful values.

## Known issues

* No pre-written code yet for several ubiquitous algorithms, such as max flow.
* Many routines intended for `graph.h` are not implemented yet.
* No templates for "hacking" on CodeForces.
* Some of the comments are still written in Polish.
