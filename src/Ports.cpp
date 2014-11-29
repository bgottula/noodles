#include "all.h"

void Ports::add(const char *name)
{
	int index = m_ports.size();
	
	if (m_names.count(name) != 0)
	{
		throw DuplicatePortException();
	}
	
	m_names.insert({name, index});
	m_ports.push_back(Port());
}

Port *Ports::find_port(const char *name)
{
	if (m_names.count(name) == 0) throw NonexistentPortException();
	int index = m_names[name];
	return &m_ports.at(index);
}

Port *InputPorts::find_port_check(const char *name)
{
	auto p = find_port(name);
	
	if (p->size() == 0) throw InputNotConnectedException();
	return p;
}

Port *OutputPorts::find_port_check(const char *name)
{
	auto p = find_port(name);
	
	if (p->size() == 0) throw OutputNotConnectedException();
	return p;
}

void InputPorts::connect(const char *name, Noodle *noodle)
{
	auto p = find_port(name);
	
	if (p->size() > 0) throw InputMultipleNoodleException();
	p->push_back(noodle);
}

void OutputPorts::connect(const char *name, Noodle *noodle)
{
	auto p = find_port(name);
	
	p->push_back(noodle);
}

size_t InputPorts::please(const char *name)
{
	auto p = find_port_check(name);
	auto n = (*p)[0];
	
	return n->count();
}

size_t OutputPorts::please(const char *name)
{
	auto p = find_port_check(name);
	
	/* find the noodle on this output port with the smallest number of free
	 * spots for samples available, and return that number */
	size_t min_free = SIZE_MAX;
	for (auto it = p->begin(); it != p->end(); ++it)
	{
		Noodle *n = *it;
		
		min_free = min(min_free, n->free());
		if (min_free == 0) break;
	}
	
	return min_free;
}

void InputPorts::get_one(const char *name, int *sample)
{
	auto p = find_port_check(name);
	auto n = (*p)[0];
	
	/* mutex block */
	{
		lock_guard<mutex> lock(n->mutex_ref());
		
		if (n->max() < 1) throw InputGetImpossibleException();
		if (n->count() < 1) throw InputGetUnsuccessfulException();
		*sample = n->pop();
	}
}

void InputPorts::get_multi(const char *name, size_t count, int *samples)
{
#warning TODO
}

void InputPorts::get_vari(const char *name, size_t count, ...)
{
#warning TODO
}

void OutputPorts::put_one(const char *name, int sample)
{
	auto p = find_port_check(name);
	
	for (auto it = p->begin(); it != p->end(); ++it)
	{
		Noodle *n = *it;
		
		/* mutex block */
		{
			lock_guard<mutex> lock(n->mutex_ref());
			
			if (n->max() < 1) throw OutputPutImpossibleException();
			if (n->free() < 1) throw OutputPutUnsuccessfulException();
			n->push(sample);
		}
	}
}

void OutputPorts::put_repeat(const char *name, size_t repeat, int sample)
{
	assert(repeat >= 1);
	auto p = find_port_check(name);
	
	for (auto it = p->begin(); it != p->end(); ++it)
	{
		Noodle *n = *it;
		
		/* mutex block */
		{
			lock_guard<mutex> lock(n->mutex_ref());
			
			if (n->max() < repeat) throw OutputPutImpossibleException();
			if (n->free() < repeat) throw OutputPutUnsuccessfulException();
			
			while (repeat-- > 0)
			{
				n->push(sample);
			}
		}
	}
}

void OutputPorts::put_multi(const char *name, size_t count, const int *samples)
{
#warning TODO
}

void OutputPorts::put_vari(const char *name, size_t count, ...)
{
#warning TODO
}
