#ifndef NOODLE_H
#define NOODLE_H

/* forward declarations */
template <typename T> class InputPort;
template <typename T> class OutputPort;

/* base noodle class (protected ctor prevents direct instantiation) */
template <typename T>
class Noodle
{
public:
	virtual ~Noodle() {}
	
	virtual bool is_qnoodle(void) const = 0;
	virtual void lock(unique_lock<mutex>& mgr) = 0;
	
	virtual size_t max(void) const = 0;
	virtual size_t count(void) const = 0;
	virtual size_t free(void) const = 0;
	
	virtual void push(const T& sample) = 0;
	virtual T pop(void) = 0;
	virtual T peek(size_t where) = 0;
	
protected:
	Noodle(OutputPort<T> *from, InputPort<T> *to)
		: m_from(from), m_to(to) {}
	
	/* currently these backref pointers are only actually used in
	 * Graph::dumpGraph so we can easily iterate over all of the noodles and
	 * see which ports they are connected to in order to print a nice diagram
	 * for the user; in no other case do we need to quickly iterate over the
	 * list of noodles in this way; they can be ripped out later if desired */
	OutputPort<T> *m_from;
	InputPort<T>  *m_to;
};

/* queue noodle: buffers a certain number of samples */
template <typename T>
class QNoodle : public Noodle<T>
{
public:
	QNoodle(size_t max, OutputPort<T> *from, InputPort<T> *to)
		: Noodle<T>(from, to), m_max(max) {}
	
	bool is_qnoodle(void) const { return true; }
	void lock(unique_lock<mutex>& mgr) { mgr = unique_lock<mutex>(m_mutex); }
	
	/* get queue's max allowed sample count */
	size_t max(void) const { return m_max; }
	/* get queue's sample count */
	size_t count(void) const { return m_queue.size(); }
	/* get queue's free space count */
	size_t free(void) const { return m_max - m_queue.size(); }
	
	/* NEEDS LOCK: push one sample onto the queue */
	void push(const T& sample);
	/* NEEDS LOCK: pop one sample from the queue */
	T pop(void);
	/* NEEDS LOCK: peek one sample from a given position in the queue */
	T peek(size_t where);
	
private:
	/* sample buffer queue */
	deque<T> m_queue;
	/* mutex to prevent simultaneous queue modification by multiple threads */
	mutex m_mutex;
	
	/* maximum number of samples that the queue is allowed to contain */
	const size_t m_max;
};

/* register noodle: holds only one sample, has init value, never blocks */
template <typename T>
class RNoodle : public Noodle<T>
{
public:
	RNoodle(T init, OutputPort<T> *from, InputPort<T> *to)
		: Noodle<T>(from, to), m_reg(init) {}
	
	bool is_qnoodle(void) const { return false; }
	void lock(unique_lock<mutex>& mgr) { mgr = unique_lock<mutex>(m_mutex); }
	
	/* register can always hold just one sample */
	size_t max(void) const { return 1; }
	/* register can always furnish one get */
	size_t count(void) const { return 1; }
	/* register can always furnish one put */
	size_t free(void) const { return 1; }
	
	/* write to the sample register */
	void push(const T& sample) { m_reg = sample; }
	/* read from the sample register */
	T pop(void) { return m_reg; }
	/* peek the sample register (same as pop) */
	T peek(size_t where) { return m_reg; }
	
private:
	/* mutex to ensure atomic loading/storing of the register value */
	mutex m_mutex;
	
	/* register containing the current sample value */
	T m_reg;
};

#define TEMPLATES
#include "../src/Noodle.cpp"
#undef TEMPLATES

#endif
