#ifndef BLOCK_H
#define BLOCK_H

/* forward declarations */
class IContainsBlocks;

/* abstract base class for block implementations */
class Block :
	public IKnowsSubclassName,
	public IContainsPorts
{
public:
	virtual ~Block() {}
	
	virtual void reset(void) = 0;
	
	virtual bool can_work(void) = 0;
	virtual void work(void) = 0;
	
	virtual void check(void) const;
	
	virtual void set_owner(IContainsBlocks *owner);
	virtual IContainsBlocks *get_owner(void);
	
protected:
	Block() {}
	
private:
	IContainsBlocks *m_owner = nullptr;
};

/* associates a name with a block */
struct NamedBlock
{
	const char *name;
	Block *block;
};

class BlockAlreadyOwnedException : public runtime_error
{
public: BlockAlreadyOwnedException(void) :
	runtime_error("Block has already had an owner assigned") {}
};
class BlockNotOwnedException : public runtime_error
{
public: BlockNotOwnedException(void) :
	runtime_error("Block does not yet have an owner assigned") {}
};

#endif
