#ifndef BLOCK_H
#define BLOCK_H

#define REGISTER_PORT(_p) register_port(#_p, &_p)

class Block
{
	friend class Graph;
	
public:
	~Block();
	
	virtual void reset(void) = 0;
	virtual void work(void) = 0;
	
	const char *name(void) const;
	
protected:
	void register_port(const char *name, Port *p);
	void list_ports(void);
	
private:
	mutable char *m_name = nullptr;
	
	vector<NamedPort> m_ports;
};

class DuplicatePortException : public runtime_error
{
public: DuplicatePortException(void) :
	runtime_error("Blocks cannot contain duplicate ports") {};
};
class DuplicatePortNameException : public runtime_error
{
public: DuplicatePortNameException(void) :
	runtime_error("Blocks must contain ports with unique names") {};
};

class NonexistentPortException : public runtime_error
{
public: NonexistentPortException(void) :
	runtime_error("Port with that name has not been registered") {};
};

#endif
