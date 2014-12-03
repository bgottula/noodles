#ifndef GRAPH_H
#define GRAPH_H

#define REGISTER_BLOCK(_b) register_block(#_b, &_b)

/* base for graphs (protected ctor prevents direct instantiation) */
class Graph
{
public:
	virtual ~Graph();
	
	//void addQNoodle(size_t queue_max, Port *from, Port *to);
	//void addRNoodle(int init, Port *from, Port *to);
	
	const char *name(void) const;
	
	//void checkGraph(void);
	
	//void dumpGraph(bool blocks = true, bool noodles = true);
	
protected:
	Graph() {}
	
	void register_block(const char *b_name, Block *b);
	//void register_noodle(Noodle& n);
	
private:
	mutable char *m_name = nullptr;
	
	vector<NamedBlock> m_blocks;
	//vector<Noodle> m_noodles;
	
	//template <typename T>
	//void addNoodle(Noodle<T> *n, Endpoint from, Endpoint to);
	
	//bool m_needCheck = true;
	//vector<Block *> m_blocks;
	//vector<Noodle *> m_noodles; // uh oh
	
	//void dumpBlocks(void);
	//void dumpNoodles(void);
	
	//const char *str_noodle(const Noodle *n);
	//const char *str_block(const Block *b);
	//const char *str_endpoint(const Endpoint *e, bool input);
	
	//StrPool m_strpool;
	//const size_t m_strlen = 128;
};

class DuplicateBlockException : public runtime_error
{
public: DuplicateBlockException(void) :
	runtime_error("Block instances may only be registered once") {};
};
class DuplicateBlockNameException : public runtime_error
{
public: DuplicateBlockNameException(void) :
	runtime_error("Block instances must be registered with unique names") {};
};

/*class EmptyGraphException : public runtime_error
{
public: EmptyGraphException(void) :
	runtime_error("Graphs must contain at least one noodle") {};
};*/

#define TEMPLATES
#include "../src/Graph.cpp"
#undef TEMPLATES

#endif
