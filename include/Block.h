#ifndef BLOCK_H
#define BLOCK_H

#define REGISTER_PORT(_p) register_port(#_p, &_p)

/* base for block classes (protected ctor prevents direct instantiation) */
class Block
{
public:
	virtual ~Block();
	
	virtual void reset(void) = 0;
	virtual void work(void) = 0;
	
	const char *name(void) const;
	
protected:
	Block() {}
	
	void register_port(const char *p_name, Port *p);
	void list_ports(void);
	
private:
	mutable char *m_name = nullptr;
	
	vector<NamedPort> m_ports;
};

/* associates a name with a port */
struct NamedBlock
{
	const char *name;
	Block *block;
};

class DuplicatePortException : public runtime_error
{
public: DuplicatePortException(void) :
	runtime_error("Port instances may only be registered once") {};
};
class DuplicatePortNameException : public runtime_error
{
public: DuplicatePortNameException(void) :
	runtime_error("Port instances must be registered with unique names") {};
};

class NonexistentPortException : public runtime_error
{
public: NonexistentPortException(void) :
	runtime_error("Port with that name has not been registered") {};
};

#endif
