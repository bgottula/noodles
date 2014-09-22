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
	Endpoint m_from;
	Endpoint m_to;
	queue<int> m_queue;
};

#endif
