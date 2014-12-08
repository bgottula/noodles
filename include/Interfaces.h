#ifndef INTERFACES_H
#define INTERFACES_H

class IKnowsSubclassName
{
public:
	virtual ~IKnowsSubclassName() {}
	
	virtual const char *name() const final
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

template <typename T>
struct Named
{
	const char *name;
	T *ptr;
};

#endif
