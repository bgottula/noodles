#ifndef NOODLE_H
#define NOODLE_H

class Noodle
{
	friend class Graph;
	
public:
	Noodle(size_t max, Endpoint from, Endpoint to)
		: m_max(max), m_from(from), m_to(to) {};
	
	/* get queue's max allowed sample count */
	size_t max(void) { return m_max; }
	/* get queue's sample count */
	size_t count(void) { return m_queue.size(); }
	/* get queue's free space count */
	size_t free(void) { return m_max - m_queue.size(); }
	
	/* get a reference to the queue mutex */
	mutex& mutex_ref(void) { return m_mutex; }
	
	/* NEEDS LOCK: push one sample onto the queue */
	void push(int sample);
	/* NEEDS LOCK: pop one sample from the queue */
	int pop(void);
	/* NEEDS LOCK: peek one sample from a given position in the queue */
	int peek(size_t where);
	
	// TODO: find out if peek really needs the mutex to be held
	
private:
	/* sample buffer queue */
	queue<int> m_queue;
	/* mutex to prevent simultaneous queue modification by multiple threads */
	mutex m_mutex;
	
	/* maximum number of samples that the queue is allowed to contain */
	const size_t m_max;
	
	/* currently these endpoint structs are only actually used in
	 * Graph::dumpGraph so we can easily iterate over all of the noodles and
	 * see which blocks they are connected to in order to print a nice diagram
	 * for the user; in no other case do we need to quickly iterate over the
	 * list of noodles in this way; they can be ripped out later if desired */
	Endpoint m_from;
	Endpoint m_to;
};

#endif
