#include "all.h"

#ifdef TEMPLATES

template <typename T>
void InputPort<T>::connect(Noodle<T> *n)
{
	if (m_noodle != nullptr) throw InputMultipleNoodleException();
	
	m_noodle = n;
}

template <typename T>
void OutputPort<T>::connect(Noodle<T> *n)
{
	if (find(m_noodles.cbegin(), m_noodles.cend(), n) != m_noodles.cend())
		throw OutputDuplicateNoodleException();
	
	m_noodles.push_back(n);
}

template <typename T>
size_t InputPort<T>::available(void) const
{
	if (m_noodle == nullptr) throw InputNotConnectedException();
	
	return m_noodle->free();
}

template <typename T>
size_t OutputPort<T>::available(void) const
{
	if (m_noodles.empty()) throw OutputNotConnectedException();
	
	auto it = min_element(m_noodles.cbegin(), m_noodles.cend(),
		[](const Noodle<T> *lhs, const Noodle<T> *rhs) {
			return (lhs->free() < rhs->free());
		});
	
	return (*it)->free();
}

template <typename T>
void InputPort<T>::get_one(T& sample)
{
	/* mutex block */
	{
		unique_lock<mutex> mgr;
		m_noodle->lock(mgr);
		
		if (m_noodle->max() < 1) throw InputGetImpossibleException();
		if (m_noodle->count() < 1) throw InputGetUnavailableException();
		sample = m_noodle->pop();
	}
}

template <typename T>
void InputPort<T>::get_arr(size_t count, T *samples)
{
	assert(count >= 1);
	
	/* mutex block */
	{
		unique_lock<mutex> mgr;
		m_noodle->lock(mgr);
		
		if (m_noodle->max() < count) throw InputGetImpossibleException();
		if (m_noodle->count() < count) throw InputGetUnavailableException();
		
		for (size_t i = 0; i < count; ++i)
		{
			samples[i] = m_noodle->pop();
		}
	}
}

template <typename T>
void InputPort<T>::get_var(size_t count, ...)
{
	assert(count >= 1);
	
	va_list v;
	va_start(v, count);
	
	/* mutex block */
	{
		unique_lock<mutex> mgr;
		m_noodle->lock(mgr);
		
		if (m_noodle->max() < count) throw InputGetImpossibleException();
		if (m_noodle->count() < count) throw InputGetUnavailableException();
		
		for (size_t i = 0; i < count; ++i)
		{
			T *sample = va_arg(v, T *);
			*sample = m_noodle->pop();
		}
	}
	
	va_end(v);
}

template <typename T>
void InputPort<T>::peek_one(size_t where, T& sample)
{
	/* mutex block */
	{
		unique_lock<mutex> mgr;
		m_noodle->lock(mgr);
		
		if (m_noodle->max() <= where) throw InputPeekImpossibleException();
		if (m_noodle->count() <= where) throw InputPeekUnavailableException();
		sample = m_noodle->peek(where);
	}
}

template <typename T>
void OutputPort<T>::put_one(const T& sample)
{
	for (auto it = m_noodles.begin(); it != m_noodles.cend(); ++it)
	{
		Noodle<T> *n = *it;
		
		/* mutex block */
		{
			unique_lock<mutex> mgr;
			n->lock(mgr);
			
			if (n->max() < 1) throw OutputPutImpossibleException();
			if (n->free() < 1) throw OutputPutUnavailableException();
			n->push(sample);
		}
	}
}

template <typename T>
void OutputPort<T>::put_repeat(size_t count, const T& sample)
{
	assert(count >= 1);
	
	for (auto it = m_noodles.begin(); it != m_noodles.cend(); ++it)
	{
		Noodle<T> *n = *it;
		
		/* mutex block */
		{
			unique_lock<mutex> mgr;
			n->lock(mgr);
			
			if (n->max() < count) throw OutputPutImpossibleException();
			if (n->free() < count) throw OutputPutUnavailableException();
			
			for (size_t i = 0; i < count; ++i)
			{
				n->push(sample);
			}
		}
	}
}

template <typename T>
void OutputPort<T>::put_arr(size_t count, const T *samples)
{
	assert(count >= 1);
	
	for (auto it = m_noodles.begin(); it != m_noodles.cend(); ++it)
	{
		Noodle<T> *n = *it;
		
		/* mutex block */
		{
			unique_lock<mutex> mgr;
			n->lock(mgr);
			
			if (n->max() < count) throw InputGetImpossibleException();
			if (n->count() < count) throw InputGetUnavailableException();
			
			for (size_t i = 0; i < count; ++i)
			{
				n->push(samples[i]);
			}
		}
	}
}

template <typename T>
void OutputPort<T>::put_var(size_t count, ...)
{
	assert(count >= 1);
	
	va_list v;
	va_start(v, count);
	
	T samples[count];
	for (size_t i = 0; i < count; ++i)
	{
		const T& sample = va_arg(v, const T&);
		samples[i] = sample;
	}
	put_arr(count, samples);
	
	va_end(v);
}

template <typename T>
void InputPort<T>::check(void) const
{
#warning FINISHME
}

template <typename T>
void OutputPort<T>::check(void) const
{
#warning FINISHME
}

#else

void Port::set_owner(Block *b)
{
	if (m_owner != nullptr) throw PortAlreadyOwnedException();
	
	m_owner = b;
}

Block *Port::get_owner(void)
{
	if (m_owner == nullptr) throw PortNotOwnedException();
	
	return m_owner;
}

#endif
