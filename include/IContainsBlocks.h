#ifndef ICONTAINSBLOCKS_H
#define ICONTAINSBLOCKS_H

class IContainsBlocks
{
public:
	virtual ~IContainsBlocks() {}
	
	virtual vector<NamedBlock>::const_iterator blocks_cbegin(void)
	{
		return m_blocks.cbegin();
	}
	virtual vector<NamedBlock>::const_iterator blocks_cend(void)
	{
		return m_blocks.cend();
	}
	
	virtual vector<NamedBlock>::const_iterator blocks_begin(void)
	{
		return m_blocks.begin();
	}
	virtual vector<NamedBlock>::const_iterator blocks_end(void)
	{
		return m_blocks.end();
	}
	
	virtual Block *find_block(const char *b_name);
	
protected:
	IContainsBlocks() {}
	
	virtual void register_block(const char *b_name, Block *b);
	//virtual void list_blocks(void);
	
private:
	vector<NamedBlock> m_blocks;
};

class NonexistentBlockException : public runtime_error
{
public: NonexistentBlockException(void) :
	runtime_error("Block with that name has not been registered") {}
};
class DuplicateBlockException : public runtime_error
{
public: DuplicateBlockException(void) :
	runtime_error("Block instances may only be registered once") {}
};
class DuplicateBlockNameException : public runtime_error
{
public: DuplicateBlockNameException(void) :
	runtime_error("Block instances must be registered with unique names") {}
};

#define REGISTER_BLOCK(_b) register_block(#_b, &_b)

#endif
