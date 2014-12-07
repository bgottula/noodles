#ifndef GRAPH_H
#define GRAPH_H

#define REGISTER_BLOCK(_b) register_block(#_b, &_b)

#define ADD_QNOODLE(_type, _max, _b_from, _p_from, _b_to, _p_to) \
	add_noodle(new QNoodle<_type>(_max, (_b_from).find_port(#_p_from), \
		(_b_to).find_port(#_p_to)))
#define ADD_RNOODLE(_type, _init, _b_from, _p_from, _b_to, _p_to) \
	add_noodle(new RNoodle<_type>(_init, (_b_from).find_port(#_p_from), \
		(_b_to).find_port(#_p_to)))

enum class GraphState
{
	SETUP,
	RUN,
};

/* base for graphs (protected ctor prevents direct instantiation) */
class Graph
{
public:
	virtual ~Graph();
	
	void check(void);
	void dump(bool blocks = true, bool noodles = true) const;
	
	const char *name(void) const;
	
protected:
	Graph() {}
	
	void register_block(const char *b_name, Block *b);
	void add_noodle(NoodleBase *n);
	
private:
	mutable const char *m_name = nullptr;
	
	vector<NamedBlock> m_blocks;
	vector<NoodleBase *> m_noodles;
	
	GraphState m_state = GraphState::SETUP;
	
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

class GraphModifiedAfterSetupException : public runtime_error
{
public: GraphModifiedAfterSetupException(void) :
	runtime_error("Graphs can only be modified when they are in the SETUP "
		"state") {};
};

class GraphDuplicateBlockException : public runtime_error
{
public: GraphDuplicateBlockException(void) :
	runtime_error("Block instances may only be registered once") {};
};
class GraphDuplicateBlockNameException : public runtime_error
{
public: GraphDuplicateBlockNameException(void) :
	runtime_error("Block instances must be registered with unique names") {};
};

class GraphDuplicateNoodleException : public runtime_error
{
public: GraphDuplicateNoodleException(void) :
	runtime_error("Noodle instances may only be registered once") {};
};

class GraphNoBlocksException : public runtime_error
{
public: GraphNoBlocksException(void) :
	runtime_error("Graphs must contain at least one block") {};
};
class GraphNoNoodlesException : public runtime_error
{
public: GraphNoNoodlesException(void) :
	runtime_error("Graphs must contain at least one noodle") {};
};

#endif
