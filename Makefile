# all header files
HEADERS:=$(shell find include/ -iname '*.h')

# object files used to link bin/noodles
OBJ_NOODLES:=obj/Block.o obj/Scheduler.o obj/main.o
# object files used to link bin/test
OBJ_TEST:=obj/Block.o obj/Scheduler.o obj/unit/BlockTest.o


# phony targets: these rules don't generate the files they name
.PHONY: all noodles test clean

# 'make' or 'make all' -> build all binaries
all: noodles test
# 'make noodles' -> build bin/noodles
noodles: bin/noodles
# 'make test' -> build bin/test
test: bin/test

# clean up (ignore failure)
clean:
	-rm -vf bin/* obj/*.o obj/unit/*.o


# binaries
bin/noodles: $(OBJ_NOODLES) Makefile
	-mkdir -vp bin
	g++ -std=gnu++11 -Wall -Wextra -g -Og -o $@ $(OBJ_NOODLES)
bin/test: $(OBJ_TEST) Makefile
	-mkdir -vp bin
	g++ -std=gnu++11 -Wall -Wextra -g -Og -o $@ $(OBJ_TEST)

# source compilation
obj/%.o: src/%.cpp include/std.h.gch $(HEADERS) Makefile
	-mkdir -vp obj/unit
	g++ -std=gnu++11 -Wall -Wextra -g -Og -I include/ -c $< -o $@

# precompiled header generation
include/std.h.gch: include/std.h Makefile
	g++ -std=gnu++11 -Wall -Wextra -g -Og -x c++-header $< -o $@
