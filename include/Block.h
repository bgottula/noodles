#ifndef BLOCK_H
#define BLOCK_H

/* base for block implementations */
class Block :
	public IKnowsSubclassName,
	public IContainsPorts
{
public:
	virtual ~Block() {}
	
	virtual void reset(void) = 0;
	
	virtual bool can_work(void) = 0;
	virtual void work(void) = 0;
	
	void check(void) const;
	
protected:
	Block() {}
};

/* associates a name with a block */
struct NamedBlock
{
	const char *name;
	Block *block;
};

#endif
