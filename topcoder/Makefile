FDICPPX := $(subst .cc,.cpp,$(FDIFILE))
FDIEXEC := $(subst .cc,.e,$(FDIFILE))
FDICPP := ../xfer/$(FDICPPX)

#FDIFILE:.cc=.cpp)

all: compile run

compile: $(FDIEXEC)

$(FDICPP): $(FDIFILE)
	../templator/templator $(FDIFILE) $(FDICPP)

$(FDIEXEC): $(FDICPP)
	rm -f $(FDIEXEC)
	g++ -Wall -g -std=c++0x -I~/kon/tc/main/ $(FDICPP) -o $(FDIEXEC) -O2

run: $(FDIEXEC)
	./$(FDIEXEC)

debug:
	gdb $(FDIEXEC)

env:
	echo $(FDIFILE)

