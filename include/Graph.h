#ifndef GRAPH_H
#define GRAPH_H

enum class GraphState
{
	SETUP,
	RUN,
};

/* base for graphs (protected ctor prevents direct instantiation) */
class Graph :
	public IKnowsSubclassName,
	public IContainsNoodles,
	public IContainsBlocks
{
public:
	virtual ~Graph();
	
	void check(void);
	void dump(bool blocks = true, bool noodles = true) const;
	
protected:
	Graph() {}
	
private:
	GraphState m_state = GraphState::SETUP;
	
	//void dumpBlocks(void);
	//void dumpNoodles(void);
	
	//const char *str_noodle(const Noodle *n);
	//const char *str_block(const Block *b);
	//const char *str_endpoint(const Endpoint *e, bool input);
	
	//StringPool m_strpool;
	//const size_t m_strlen = 128;
};

class GraphModifiedAfterSetupException : public runtime_error
{
public: GraphModifiedAfterSetupException(void) :
	runtime_error("Graphs can only be modified when they are in the SETUP "
		"state") {}
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
