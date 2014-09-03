bin/noodles: obj/main.o obj/Block.o obj/Scheduler.o
	g++ -std=gnu++11 -Wall -Wextra -g -Og -o $@ $^

obj/std.h.gch: include/std.h
	g++ -std=gnu++11 -Wall -Wextra -g -Og -x c++-header $< -o $@

obj/%.o: src/%.cpp obj/std.h.gch include/*.h
	g++ -std=gnu++11 -Wall -Wextra -g -Og -I include/ -c $< -o $@

obj/unit/%.o: src/unit/%.cpp obj/std.h.gch include/*.h
	g++ -std=gnu++11 -Wall -Wextra -g -Og -I include/ -c $< -o $@

.PHONY: clean
clean:
	rm -vf bin/* obj/* obj/unit/*

test: obj/Block.o obj/Scheduler.o obj/unit/BlockTest.o
	g++ -std=gnu++11 -Wall -Wextra -g -Og -o bin/test $^
