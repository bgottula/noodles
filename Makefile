noodles: main.o Block.o Scheduler.o
	g++ -std=gnu++11 -Wall -Wextra -g -Og -o $@ $^

std.h.gch: std.h
	g++ -std=gnu++11 -Wall -Wextra -g -Og -x c++-header $<

%.o: %.cpp std.h.gch
	g++ -std=gnu++11 -Wall -Wextra -g -Og -c $<

.PHONY: clean
clean:
	rm -vf noodles *.o *.gch
