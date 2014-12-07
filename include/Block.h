#ifndef BLOCK_H
#define BLOCK_H

#define REGISTER_PORT(_p) register_port(#_p, &_p)

/* base for block classes (protected ctor prevents direct instantiation) */
class Block
{
public:
	virtual ~Block();
	
	virtual void reset(void) = 0;
	
	virtual bool can_work(void) = 0;
	virtual void work(void) = 0;
	
	void check(void) const;
	
	Port *find_port(const char *p_name);
	
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

class BlockDuplicatePortException : public runtime_error
{
public: BlockDuplicatePortException(void) :
	runtime_error("Port instances may only be registered once") {};
};
class BlockDuplicatePortNameException : public runtime_error
{
public: BlockDuplicatePortNameException(void) :
	runtime_error("Port instances must be registered with unique names") {};
};

class BlockNonexistentPortException : public runtime_error
{
public: BlockNonexistentPortException(void) :
	runtime_error("Port with that name has not been registered") {};
};

#endif
