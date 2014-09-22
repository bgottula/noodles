#ifndef BLOCK_H
#define BLOCK_H

class Block
{
	friend class Graph;
	
public:
	virtual void reset(void) = 0;
	virtual void work(void) = 0;
	const char *name(void);
	
protected:
	InputPorts inputs;
	OutputPorts outputs;
};

#endif
