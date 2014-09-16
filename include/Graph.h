#ifndef GRAPH_H
#define GRAPH_H

/* NoodleGraph is an adjacency list where the vertices are of type Block *, and
 * the edges are of type std::pair <int,int>; use std::vector for storage */
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
	Block *, std::pair <int, int> > NoodleGraph;

/* vertex_t and edge_t are essentially reference types for vertices and edges.
 * use m_graph[...] with one of these references to access the data */
typedef boost::graph_traits <NoodleGraph>::vertex_descriptor vertex_t;
typedef boost::graph_traits <NoodleGraph>::edge_descriptor edge_t;

class Graph
{
    public:
        void addNoodle(Noodle *n);
        int checkGraph(void);
        void dumpGraph(void);
        void run(void);

    private:
    	bool m_needCheck = true;
        NoodleGraph m_graph;
};

class EmptyGraphException : public std::runtime_error
{
	public: EmptyGraphException(void) :
		runtime_error("Graphs must contain at least one noodle") {};
};
class DuplicateNoodleException : public std::runtime_error
{
	public: DuplicateNoodleException(void) :
		runtime_error("Exact duplicate noodles are not allowed") {};
};
class InputMultipleNoodleException : public std::runtime_error
{
	public: InputMultipleNoodleException(void) :
		runtime_error("Input ports cannot be connected to multiple noodles") {};
};
class GraphInvalidException : public std::runtime_error
{
	public: GraphInvalidException(void) :
		runtime_error("Graph validation failed") {};
};

#endif
