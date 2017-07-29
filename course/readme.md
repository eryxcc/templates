## WARNING

This is a work in progress, and in a very early state.

## Introduction

There is a common opinion that one should not start learning programming with C++. 
However, I believe that this opinion comes from some misconceptions programmers
often have about C++. C++ is a constantly evolving language, and many things
which were previously true about it are no longer true.

They claim that C++ is a very complex language. While this is definitely true,
almost noone learns C++ in its entirety. One does learn just a subset of this
language. Companies using C++ often have a listing of language features which
are allowed, and features which are banned, because it is better not to force
all the programmers in the company to be familiar with them.

Some people recommend starting with C. Basically, C is a subset of C++,
so this goes in the right direction. 
However, C is very ugly in some areas, and 
I do believe that there are subsets of C++ which are better than C
for starting programming.

It is also good to note that, when working with a programming language,
you extend it. You create new functions which extend the capabilities
of the language. This course is based on easy.cpp, which is a C++
file containing easy workarounds for some more complicated aspects of C++.
Of course this is still programming in C++.

However, C has one big advantage over other subsets of C++: the 
Kernighan and Ritchie book, _Programming in C_. It is a very good book,
but as far as I can tell, no good course exists to teach my favorite
easy subset of C++. The purpose of this course is to fix this.

## What will I be able to do after this programming course?

* Solve puzzles. Computers are great in solving many kinds of puzzles,
and learning programming will allow you to use this. The algorithmic
programming contests are basically contests at solving puzzles, except
that you are allowed to use the computer, rather than doing everything
in your head. And as with other sports, they are fun even if you are
not the best in the world. The course will prepare you to start your
adventure with programming contests, or to solve puzzles and other
problems outside of the programming contest form.

* Create programs drawing pictures. This is what I have started with
myself: writing programs which draw nice pictures. 
Unfortunately, while on typical computers from 80's,
running a program drawing a line was a matter of simply writting
`LINE 10,10,200,200`, today's computers are complex enough that
this is no longer that simple, and generally becomes harder
and harder. This course contains libraries which should make
this somewhat easier again. Once you learn how to draw pictures,
you can go further: create games, animations, visualizations, graphs, and other fun stuff.
                                                           
* Generally learning programming is useful to test and improve your
skills in logical thinking.

* Learning programming is useful as a way to automate your tasks.
If you have ever done stupid, repetitive work on your computer, it
is possible that programming could have made the job easier.
However, C++ does not excel here -- it might be more appropriate
to learn shell or Python. (Learn these languages first might
be useful for other purposes; if you go the Linux route as suggested,
you will learn shell along the way.)

* Programming is fun! Of course it can be also used to earn money. But 
if your point is to earn money, this is probably not the best course.
Other courses, and other languages than C++, will allow you to make
money faster.

## Why start in C++, not another language?

The introduction has already explained why not start with C. Other
popular programming languages include Python, Java and C#. So what
are the differences?

* I believe Python is actually quite a good language to start
programming. It is an interpreted language, one can type a command and 
have it immediately executed -- you instantly see the effects! It is
also widely used in practice, and has very nice syntax. It has many
great libraries, too. I think it might be
actually a very good idea to read this section to know the disadvantages
of Python, then learn Python from a good course, then proceed with this
course of C++.

* C++ is statically typed and has
statically bound variables. This means that there is a big class of
bugs which will be caught by the static type checker, running which is
an integral part of the C++ programming process. 
In Python, the interpreter will not catch these bugs, so you
will actually have to run the program, test it, find that it does not
work in some situation, and look for the reason. This is necessary in
all languages
(functional programming languages are famous for having static
type checkers so good that even quite complex programs often work
correctly once the type checking is passed -- but still, this
is not infassible) but still, C++ makes this part easier.

* C++ is generally believed to be a very fast language. C++ programs
run fast. In many applications it is not very relevant, but it could
be important if you are interested in programming contests, games
programming, or when doing more complex calculations. Generally C#
and Java are fast enough for most uses, but Python is comparably
slow. Data scientists use Python quite successfully for their
complex computations, but this requires a lot of thought. C++
is in fact the most popular language used in programming contests.

* C++ is a multi-paradigm language. People say that C++ is based
on the object oriented programming (OOP) paradigm -- but this is an
outdated opinion to some extent. OOP was the first feature not in
C that was added to C++. I believe OOP is quite a complex concept,
and it is better to start learning programming without it. It is not
necessary in many areas of programming, including the ones which
are the focus of this course. (Though, it
is very useful in many areas where the money is.)
** Languages such as C# or Java are popular, but I believe they focus
too much on object oriented programming in my opinion.

## Running your first program

I believe that, among the operating systems, Linux is most friendly
to programmers. Because of its shell. A shell allows you to
talk to your computer, to issue commands to it. In Linux,  
you can install GNU C++ and an editor (say, Atom) with a single command:

   sudo apt-get install g++ atom

Lots of automatization can be done without any programming in C++,
just by writing shell commands. The Windows shell was traditionally
extremely weak compared to Linux, although it becomes a bit better
in the more recent versions. OSX is better than Windows, as it has
a powerful shell, but still, installing software is more difficult
than issuing a single command.

Of course for people already familiar with Windows, but not with
Linux, Linux and shell are just another things to learn -- but if
you are learning programming, why not learn this too? :)
The course will assume Linux, if you are using another OS, you will have to
find out how to make the compiler work on your own. One note:
since it is not possible to run two operating systems at once,
it might be the easiest to run Linux from a CD, or a virtual
machine, rather than to install it on your computer.
 
Let's create a workspace first:

  mkdir course

Change the working directory to course:

  cd course
  
Put easy.cpp and hello.cpp in this directory. The manual way is to
run the editor (say, Atom), open the webpages containing the 
files in your browser, copy the contents, paste them into the editor,
and save the files. Alternatively, simply run the following command,
which will download the files from the course:

  sudo apt-get install wget
  wget ...
  wget ...

Now, run the C++ compiler:

  g++ hello.cpp -o hello

And run the program:

  ./hello
  
Look at hello.cpp, should be easy to understand. You can try to modify it and
see whether it works. (Do not look at
easy.cpp though -- this one translates simple things into bare C++,
and it is quite difficult itself.) Happy coding!

 