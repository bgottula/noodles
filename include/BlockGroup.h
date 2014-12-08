#ifndef BLOCKGROUP_H
#define BLOCKGROUP_H

/* forward declarations */
class IContainsBlockGroups;

/* abstract base class for block group implementations */
class BlockGroup :
	public IHasOwner<IContainsBlockGroups>,
	public IKnowsSubclassName,
	public IContainsBlocks,
	public IContainsPorts,
	public IContainsNoodles
{
public:
	virtual ~BlockGroup() {}
	
	virtual void check() const final;
	virtual void dump(int level = 0) const final;
	
protected:
	BlockGroup() {}
};

#endif
