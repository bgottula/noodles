#ifndef INTERFACES_H
#define INTERFACES_H

class IKnowsSubclassName
{
public:
	virtual ~IKnowsSubclassName() {}
	
	virtual const char *name() const
	{
		if (m_name == nullptr)
		{
			m_name = demangle(typeid(*this).name());
		}
		
		return m_name;
	}
	
protected:
	IKnowsSubclassName() {}
	
private:
	mutable const char *m_name = nullptr;
};

class AlreadyOwnedException : public runtime_error
{
public: AlreadyOwnedException() : runtime_error(
	"This object has already had an owner set") {}
};
class NotOwnedException : public runtime_error
{
public: NotOwnedException() : runtime_error(
	"This object has not had an owner set") {}
};

template <typename T>
class IHasOwner
{
public:
	virtual ~IHasOwner() {}
	
	virtual const T *get_owner() const
	{
		if (m_owner == nullptr) throw NotOwnedException();
		
		return m_owner;
	}
	
	virtual void set_owner(const T *owner)
	{
		if (m_owner != nullptr) throw AlreadyOwnedException();
		
		m_owner = owner;
	}
	
protected:
	IHasOwner() {}
	
private:
	const T *m_owner = nullptr;
};

#endif
