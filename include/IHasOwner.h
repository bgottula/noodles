#ifndef IHASOWNER_H
#define IHASOWNER_H

template <typename T>
class IHasOwner
{
public:
	virtual ~IHasOwner() {}
	
	virtual void check() const;
	
	virtual const T *get_owner() const;
	virtual void set_owner(const T *owner);
	
protected:
	IHasOwner() {}
	
private:
	const T *m_owner = nullptr;
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

#endif
