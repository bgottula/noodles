#ifndef NOODLE_H
#define NOODLE_H

class Noodle
{
public:
	Noodle(const char *fromPort, const char *toPort)
		: m_fromPort(fromPort), m_toPort(toPort) {}
	
	bool empty(void) { return m_queue.empty(); }
	void push(int sample) { m_queue.push(sample); }
	int peek(void) { return m_queue.front(); }
	int pop(void);
	
	const char *m_fromPort;
	const char *m_toPort;
	
private:
	queue<int> m_queue;
};

#endif
