#ifndef ICONTAINSBLOCKS_H
#define ICONTAINSBLOCKS_H

class IContainsBlocks
{
public:
	virtual ~IContainsBlocks() {}
	
	virtual void check() const;
	
	vector<NamedBlock>::iterator blocks_begin()
	{ return m_blocks.begin(); }
	vector<NamedBlock>::iterator blocks_end()
	{ return m_blocks.end(); }
	
	vector<NamedBlock>::const_iterator blocks_cbegin() const
	{ return m_blocks.cbegin(); }
	vector<NamedBlock>::const_iterator blocks_cend() const
	{ return m_blocks.cend(); }
	
	vector<NamedBlock>::size_type blocks_size() const
	{ return m_blocks.size(); }
	bool blocks_empty() const
	{ return m_blocks.empty(); }
	
	virtual Block *find_block(const char *b_name);
	
protected:
	IContainsBlocks() {}
	
	virtual void register_block(const char *b_name, Block *b);
	//virtual void list_blocks();
	
private:
	vector<NamedBlock> m_blocks;
};

class NonexistentBlockException : public runtime_error
{
public: NonexistentBlockException() : runtime_error(
	"Block with that name has not been registered") {}
};
class DuplicateBlockException : public runtime_error
{
public: DuplicateBlockException() : runtime_error(
	"Block instances may only be registered once") {}
};
class DuplicateBlockNameException : public runtime_error
{
public: DuplicateBlockNameException() : runtime_error(
	"Block instances must be registered with unique names") {}
};

#define REGISTER_BLOCK(_b) register_block(#_b, &_b)

#endif
