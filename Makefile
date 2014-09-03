# all header files
HEADERS:=$(shell find include/ -iname '*.h')

# object files used to link bin/noodles
OBJ_NOODLES:=src/Block.o src/Scheduler.o src/main.o
# object files used to link bin/test
OBJ_TEST:=src/Block.o src/Scheduler.o src/unit/BlockTest.o


# phony targets: these rules don't generate the files they name
.PHONY: noodles test dirs clean

# 'make' or 'make noodles' -> builds bin/noodles
noodles: bin/noodles
# 'make test' -> builds bin/test
test: bin/test

# make output directories (ignore failure)
dirs:
	-mkdir -vp bin

# clean up (ignore failure)
clean:
	-rm -vf bin/* src/*.o src/unit/*.o


# binaries
bin/noodles: $(OBJ_NOODLES) dirs Makefile
	g++ -std=gnu++11 -Wall -Wextra -g -Og -o $@ $(OBJ_NOODLES)
bin/test: $(OBJ_TEST) dirs Makefile
	g++ -std=gnu++11 -Wall -Wextra -g -Og -o $@ $(OBJ_TEST)

# source compilation
%.o: %.cpp include/std.h.gch $(HEADERS) Makefile
	g++ -std=gnu++11 -Wall -Wextra -g -Og -I include/ -c $< -o $@

# precompiled header generation
include/std.h.gch: include/std.h Makefile
	g++ -std=gnu++11 -Wall -Wextra -g -Og -x c++-header $< -o $@
