#ifndef GRAPH_H
#define GRAPH_H

/* forward declarations so we can declare class pointers here */
class Block;
class Noodle;

class Graph
{
	friend class Scheduler;
	friend class RoundRobinScheduler;
	friend class ThreadedScheduler;
	friend class InteractiveScheduler;
	
public:
	void addQNoodle(size_t queue_max, Endpoint from, Endpoint to);
	void addRNoodle(int init, Endpoint from, Endpoint to);
	
	void checkGraph(void);
	
	void dumpGraph(bool blocks = true, bool noodles = true);
	
private:
	void addNoodle(Noodle *n, Endpoint from, Endpoint to);
	
	bool m_needCheck = true;
	vector<Block *> m_blocks;
	vector<Noodle *> m_noodles;
	
	void dumpBlocks(void);
	void dumpNoodles(void);
	
	const char *str_noodle(const Noodle *n);
	const char *str_block(const Block *b);
	const char *str_endpoint(const Endpoint *e, bool input);
	
	StrPool m_strpool;
	const size_t m_strlen = 128;
};

class EmptyGraphException : public runtime_error
{
public: EmptyGraphException(void) :
	runtime_error("Graphs must contain at least one noodle") {};
};

#endif
