#include "all.h"

#ifdef TEMPLATES

template <typename T>
void QNoodle<T>::push(const T& sample)
{
	assert(m_queue.size() <= m_max);
	m_queue.push_back(sample);
}

template <typename T>
T QNoodle<T>::pop(void)
{
	T sample = m_queue.front();
	m_queue.pop_front();
	return sample;
}

template <typename T>
T QNoodle<T>::peek(size_t where)
{
	T sample = m_queue[where];
	return sample;
}

#endif
