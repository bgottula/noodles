#ifndef NOODLE_T_H
#define NOODLE_T_H

template <typename T>
Noodle<T>::Noodle(Port *from, Port *to)
{
	OPort *o_from = dynamic_cast<OPort *>(from);
	IPort *i_to   = dynamic_cast<IPort *>(to);
	
	if (o_from == nullptr) throw NoodleFromPortNotOutputException();
	if (i_to == nullptr) throw NoodleToPortNotInputException();
	
	m_from = dynamic_cast<OutputPort<T> *>(o_from);
	m_to   = dynamic_cast<InputPort<T> *>(i_to);
	
	if (m_from == nullptr) throw NoodleFromPortWrongTypeException();
	if (m_to == nullptr) throw NoodleToPortWrongTypeException();
	
	int status;
	char *this_demangled = abi::__cxa_demangle(typeid(*this).name(),
		nullptr, nullptr, &status);
	assert(status == 0);
	char *from_demangled = abi::__cxa_demangle(typeid(*m_from).name(),
		nullptr, nullptr, &status);
	assert(status == 0);
	char *to_demangled = abi::__cxa_demangle(typeid(*m_to).name(),
		nullptr, nullptr, &status);
	assert(status == 0);
	
	debug(AT_BLD "%s::ctor:" AT_RST " from %s (@ %p) to %s (@ %p)\n",
		this_demangled, from_demangled, m_from, to_demangled, m_to);
	
	std::free(this_demangled);
	std::free(from_demangled);
	std::free(to_demangled);
	
	m_from->connect(this);
	m_to->connect(this);
}

template<typename T>
void Noodle<T>::check(void) const
{
#warning FINISHME
}

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
