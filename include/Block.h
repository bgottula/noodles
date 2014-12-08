#ifndef BLOCK_H
#define BLOCK_H

/* forward declarations */
class IContainsBlocks;

/* abstract base class for block implementations */
class Block :
	public IHasOwner<IContainsBlocks>,
	public IKnowsSubclassName,
	public IContainsPorts
{
public:
	virtual ~Block() {}
	
	virtual void check() const final;
	virtual void dump(int level = 0) const final;
	
	virtual void reset() = 0;
	
	virtual bool can_work() = 0;
	virtual void work() = 0;
	
protected:
	Block() {}
};

#endif
