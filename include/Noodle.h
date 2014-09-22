#ifndef NOODLE_H
#define NOODLE_H

/* forward declaration required because Noodle.h needs to know about
 * class Block, while Block.h needs to know about class Noodle */
class Block;

struct Endpoint
{
	Block *block;
	const char *port;
};

class Noodle
{
public:
	Noodle(Endpoint from, Endpoint to)
		: m_from(from), m_to(to) {}
	
	bool empty(void) { return m_queue.empty(); }
	void push(int sample) { m_queue.push(sample); }
	int peek(void) { return m_queue.front(); }
	int pop(void);
	
private:
	Endpoint m_from;
	Endpoint m_to;
	queue<int> m_queue;
};

#endif
