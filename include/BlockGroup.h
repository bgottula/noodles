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
	
	virtual void check() const;
	
protected:
	BlockGroup() {}
};

#endif
