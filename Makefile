# object files used to link all binaries
OBJ_COMMON:=obj/Block.o obj/Scheduler.o
# object files used to link bin/noodles
OBJ_NOODLES:=$(OBJ_COMMON) obj/main.o
# object files used to link bin/test
OBJ_TEST:=$(OBJ_COMMON) obj/unit/BlockTest.o

# all header files
HEADERS:=include/*.h

# g++ options
GCC_FLAGS:=-std=gnu++11 -Wall -Wextra -g -Og -I include


# phony targets: these rules don't generate the files they name
.PHONY: all noodles test clean

# 'make' or 'make all' -> build all binaries
all: noodles test
# 'make noodles' -> build bin/noodles
noodles: bin/noodles
# 'make test' -> build bin/test and run it
test: bin/test
	bin/test

# clean up build products
clean:
	-rm -vrf bin obj include/std.h.gch


# binary linking
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
