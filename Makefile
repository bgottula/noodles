# all header files
HEADERS:=include/*.h
# object files used to link bin/noodles
OBJ_NOODLES:=obj/Block.o obj/Scheduler.o obj/main.o
# object files used to link bin/test
OBJ_TEST:=obj/Block.o obj/Scheduler.o obj/unit/BlockTest.o

# g++ options
GCC_FLAGS:=-std=gnu++11 -Wall -Wextra -g -Og -I include


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
	-rm -vrf bin obj include/std.h.gch


# binaries
bin/noodles: $(OBJ_NOODLES) Makefile
	-mkdir -p $(@D)
	g++ $(GCC_FLAGS) -o $@ $(OBJ_NOODLES)
bin/test: $(OBJ_TEST) Makefile
	-mkdir -p $(@D)
	g++ $(GCC_FLAGS) -o $@ $(OBJ_TEST)

# source compilation
obj/%.o: src/%.cpp include/std.h.gch $(HEADERS) Makefile
	-mkdir -p $(@D)
	g++ $(GCC_FLAGS) -c -o $@ $<

# precompiled header generation
include/std.h.gch: include/std.h Makefile
	g++ $(GCC_FLAGS) -x c++-header -o $@ $<
