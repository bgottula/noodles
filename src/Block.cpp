#include "all.h"

void Ports::add(const char *name)
{
	int index = m_ports.size();
	
	if (m_names.count(name) != 0)
	{
		throw DuplicatePortException();
	}
	
	m_names.insert({name, index});
	m_ports.push_back(new vector<Noodle *>());
}

void Inputs::connect(const char *name, Noodle *noodle)
{
	if (m_names.count(name) == 0)
	{
		throw NonexistentPortException();
	}
	
	int index = m_names[name];
	
	if (m_ports[index]->count > 0)
	{
		throw InputMultipleNoodleException();
	}
	
	/* use vector::at instead of vector::operator[] so we get bounds checking */
	m_ports.at(index)->push_back(noodle);
}

bool Inputs::get(const char *name, int *sample)
{
	if (m_names.count(name) == 0)
	{
		throw NonexistentPortException();
	}
	
	int index = m_names[name];
	auto noodles = m_ports[index];
	
	if (noodles->count == 0)
	{
		throw InputNotConnectedException();
	}
	
	Noodle *n = noodles->at(0);
	
	if (n->empty())
	{
		return false;
	}
	else
	{
		*sample = n->pop();
		return true;
	}
}

void Outputs::connect(const char *name, Noodle *noodle)
{
	if (m_names.count(name) == 0)
	{
		throw NonexistentPortException();
	}
	
	int index = m_names[name];
	
	/* use vector::at instead of vector::operator[] so we get bounds checking */
	m_ports.at(index)->push_back(noodle);
}

void Outputs::put(const char *name, int sample)
{
	if (m_names.count(name) == 0)
	{
		throw NonexistentPortException();
	}
	
	int index = m_names[name];
	auto noodles = m_ports[index];
	
	if (noodles->count == 0)
	{
		throw OutputNotConnectedException();
	}
	
	for (auto it = noodles->begin(); it != noodles->end(); ++it)
	{
		Noodle *n = *it;
		
		n->push(sample);
	}
}

const char *Block::name(void)
{
	int status;
	const char *name = abi::__cxa_demangle(typeid(*this).name(),
		nullptr, nullptr, &status);
	
	assert(status == 0);
	
	/* this pointer should be free'd by the caller to avoid leaking memory */
	return name;
}
