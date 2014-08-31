noodles: main.o Block.o Scheduler.o
	g++ -std=gnu++11 -g -Og -o $@ $^

%.o: %.cpp
	g++ -std=gnu++11 -g -Og -c $<

.PHONY: clean
clean:
	rm -vf noodles *.o
