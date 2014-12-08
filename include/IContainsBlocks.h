#ifndef ICONTAINSBLOCKS_H
#define ICONTAINSBLOCKS_H

class IContainsBlocks
{
public:
	virtual ~IContainsBlocks() {}
	
	virtual void check() const;
	virtual void dump(int level = 0) const;
	
	virtual Block *find_block(const char *b_name) final;
	
protected:
	IContainsBlocks() {}
	
	virtual void register_block(const char *b_name, Block *b) final;
	//virtual void list_blocks() final;
	
private:
	vector<Named<Block>> m_blocks;
	
	typedef vector<Named<Block>>::iterator       v_iter;
	typedef vector<Named<Block>>::const_iterator v_citer;
	typedef vector<Named<Block>>::size_type      v_size;
	
public:
	v_iter blocks_begin()         { return m_blocks.begin(); }
	v_iter blocks_end()           { return m_blocks.end(); }
	
	v_citer blocks_cbegin() const { return m_blocks.cbegin(); }
	v_citer blocks_cend()   const { return m_blocks.cend(); }
	
	v_size blocks_size()    const { return m_blocks.size(); }
	bool   blocks_empty()   const { return m_blocks.empty(); }
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
