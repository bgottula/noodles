#ifndef IHASOWNER_T_H
#define IHASOWNER_T_H

template <typename T>
void IHasOwner<T>::check() const
{
	debug(AT_BLD "IHasOwner<%s>::check" AT_RST "\n",
		demangle(typeid(T).name()));
	
#warning FINISHME
}

template <typename T>
const T *IHasOwner<T>::get_owner() const
{
	if (m_owner == nullptr) throw NotOwnedException();
	
	return m_owner;
}

template <typename T>
void IHasOwner<T>::set_owner(const T *owner)
{
	if (m_owner != nullptr) throw AlreadyOwnedException();
	
	m_owner = owner;
}

#endif
