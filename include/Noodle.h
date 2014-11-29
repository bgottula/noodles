#ifndef NOODLE_H
#define NOODLE_H

/* generic noodle class */
class Noodle
{
	friend class Graph;
	
public:
	Noodle(Endpoint from, Endpoint to)
		: m_from(from), m_to(to) {};
	
	virtual size_t max(void) = 0;
	virtual size_t count(void) = 0;
	virtual size_t free(void) = 0;
	
	virtual void push(int sample) = 0;
	virtual int pop(void) = 0;
	virtual int peek(size_t where) = 0;
	
protected:
	/* currently these endpoint structs are only actually used in
	 * Graph::dumpGraph so we can easily iterate over all of the noodles and
	 * see which blocks they are connected to in order to print a nice diagram
	 * for the user; in no other case do we need to quickly iterate over the
	 * list of noodles in this way; they can be ripped out later if desired */
	Endpoint m_from;
	Endpoint m_to;
};

/* queue noodle: buffers a certain number of samples */
class QNoodle : public Noodle
{
	friend class Graph;
	
public:
	QNoodle(size_t max, Endpoint from, Endpoint to)
		: Noodle(from, to), m_max(max) {};
	
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
	
#warning TODO
	// TODO: find out if peek really needs the mutex to be held
	
private:
	/* sample buffer queue */
	deque<int> m_queue;
	/* mutex to prevent simultaneous queue modification by multiple threads */
	mutex m_mutex;
	
	/* maximum number of samples that the queue is allowed to contain */
	const size_t m_max;
};

/* register noodle: holds only one sample, has init value, never blocks */
class RNoodle : public Noodle
{
	friend class Graph;
	
public:
	RNoodle(int init, Endpoint from, Endpoint to)
		: Noodle(from, to), m_reg(init) {};
	
	/* register can always hold just one sample */
	size_t max(void) { return 1; }
	/* register can always furnish one get */
	size_t count(void) { return 1; }
	/* register can always furnish one put */
	size_t free(void) { return 1; }
	
	/* write to the sample register */
	void push(int sample) { m_reg = sample; }
	/* read from the sample register */
	int pop(void) { return m_reg; }
	/* peek the sample register (same as pop) */
	int peek(size_t where) { return m_reg; }
	
private:
	/* register containing the current sample value */
	int m_reg;
};

#endif
