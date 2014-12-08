#ifndef ICONTAINSBLOCKGROUPS_H
#define ICONTAINSBLOCKGROUPS_H

class IContainsBlockGroups
{
public:
	virtual ~IContainsBlockGroups() {}
	
	virtual void check() const;
	
	virtual BlockGroup *find_group(const char *g_name);
	
protected:
	IContainsBlockGroups() {}
	
	virtual void register_group(const char *g_name, BlockGroup *g);
	//virtual void list_groups();
	
private:
	vector<Named<BlockGroup>> m_groups;
	
	typedef vector<Named<BlockGroup>>::iterator       v_iter;
	typedef vector<Named<BlockGroup>>::const_iterator v_citer;
	typedef vector<Named<BlockGroup>>::size_type      v_size;
	
public:
	v_iter groups_begin()         { return m_groups.begin(); }
	v_iter groups_end()           { return m_groups.end(); }
	
	v_citer groups_cbegin() const { return m_groups.cbegin(); }
	v_citer groups_cend()   const { return m_groups.cend(); }
	
	v_size groups_size()    const { return m_groups.size(); }
	bool   groups_empty()   const { return m_groups.empty(); }
};

class NonexistentBlockGroupException : public runtime_error
{
public: NonexistentBlockGroupException() : runtime_error(
	"Block group with that name has not been registered") {}
};
class DuplicateBlockGroupException : public runtime_error
{
public: DuplicateBlockGroupException() : runtime_error(
	"Block group instances may only be registered once") {}
};
class DuplicateBlockGroupNameException : public runtime_error
{
public: DuplicateBlockGroupNameException() : runtime_error(
	"Block group instances must be registered with unique names") {}
};

#define REGISTER_GROUP(_g) register_group(#_g, &_g)

#endif
