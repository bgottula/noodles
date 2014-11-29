#ifndef BLOCK_H
#define BLOCK_H

class Block
{
	friend class Graph;
	
public:
	~Block();
	
	virtual void reset(void) = 0;
	virtual void work(void) = 0;
	const char *name(void) const;
	
protected:
	InputPorts inputs;
	OutputPorts outputs;
	
private:
	mutable char *m_name = nullptr;
};

#endif
