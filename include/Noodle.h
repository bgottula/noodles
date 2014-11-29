#ifndef NOODLE_H
#define NOODLE_H

class Noodle
{
	friend class Graph;
	
public:
	Noodle(Endpoint from, Endpoint to)
		: m_from(from), m_to(to) {}
	
	bool empty(void) { return m_queue.empty(); }
	void push(int sample) { m_queue.push(sample); }
	int pop(void);
	
private:
	/* currently these endpoint structs are only actually used in
	 * Graph::dumpGraph so we can easily iterate over all of the noodles and
	 * see which blocks they are connected to in order to print a nice diagram
	 * for the user; in no other case do we need to quickly iterate over the
	 * list of noodles in this way; they can be ripped out later if desired */
	Endpoint m_from;
	Endpoint m_to;
	queue<int> m_queue;
};

#endif
