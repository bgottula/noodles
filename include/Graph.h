#ifndef GRAPH_H
#define GRAPH_H

/* forward declarations so we can declare class pointers here */
class Block;
class Noodle;

class Graph
{
public:
	void addNoodle(size_t queue_max, Endpoint from, Endpoint to);
	int checkGraph(void);
	void dumpGraph(void);
	void run(void);
	
private:
	bool m_needCheck = true;
	unordered_set<Block *> m_blocks;
	unordered_set<Noodle *> m_noodles;
};

class EmptyGraphException : public runtime_error
{
public: EmptyGraphException(void) :
	runtime_error("Graphs must contain at least one noodle") {};
};
class GraphInvalidException : public runtime_error
{
public: GraphInvalidException(void) :
	runtime_error("Graph validation failed") {};
};

#endif
