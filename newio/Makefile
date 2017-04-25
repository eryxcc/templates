all: normal threaded

templator: templator.cpp
	g++ -std=c++0x templator.cpp -o templator

solution.cpp: work.cpp templator topcoder.h
	touch solution.cpp; cat solution.cpp >> backups
	./templator work.cpp solution.cpp

normal: solution.cpp
	g++ -std=c++0x solution.cpp -O2 -o normal

debug: solution.cpp
	g++ -std=c++0x solution.cpp -O2 -o debug -DDEBUG

threaded.cpp: solution.cpp threadengine.cpp
	sed "s/\/\/ proceed/if(!checkcase()) return;/" < solution.cpp | sed "s/\/\/Eryx/#include \"threadengine.cpp\"/" | sed "s/\/\/ finish/finish();/"> threaded.cpp

threaded: threaded.cpp
	g++ -std=c++0x threaded.cpp -O2 -o threaded

t.out: normal t.in
	./normal < t.in
