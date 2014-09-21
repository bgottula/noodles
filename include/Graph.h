#ifndef GRAPH_H
#define GRAPH_H

/* NoodleGraph is an adjacency list where the vertices are of type Block *, and
 * the edges are of type Noodle *; use vector for storage */
typedef adjacency_list<vecS, vecS, directedS, Block *, Noodle *> NoodleGraph;

/* vertex_t and edge_t are essentially reference types for vertices and edges.
 * use m_graph[...] with one of these references to access the data */
typedef graph_traits<NoodleGraph>::vertex_descriptor vertex_t;
typedef graph_traits<NoodleGraph>::edge_descriptor edge_t;

struct Endpoint
{
	Block *block;
	const char *port;
};

class Graph
{
public:
	void addNoodle(Endpoint from, Endpoint to);
	int checkGraph(void);
	void dumpGraph(void);
	void run(void);
	
private:
	bool m_needCheck = true;
	NoodleGraph m_graph;
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
