#include "all.h"

void Ports::add(const char *name)
{
	int index = m_ports.size();
	
	// TODO: prevent duplicate names
	
	m_names.insert({name, index});
	m_ports.push_back(new vector<Noodle *>());
}

void Inputs::connect(const char *name, Noodle *noodle)
{
	// TODO: ensure that an input with this name exists
	int index = m_names[name];
	
	// TODO: ensure that the index isn't out of bounds
	// TODO: ensure that this port isn't already connected
	m_ports[index]->push_back(noodle);
}

bool Inputs::get(const char *name, int *sample)
{
	// TODO: ensure that an input with this name exists
	int index = m_names[name];
	auto noodles = m_ports[index];
	
	if ((*noodles)[0]->empty())
	{
		return false;
	}
	else
	{
		*sample = (*noodles)[0]->pop();
		return true;
	}
}

void Outputs::connect(const char *name, Noodle *noodle)
{
	// TODO: ensure that an output with this name exists
	int index = m_names[name];
	
	// TODO: ensure that the index isn't out of bounds
	m_ports[index]->push_back(noodle);
}

void Outputs::put(const char *name, int sample)
{
	// TODO: ensure that an output with this name exists
	int index = m_names[name];
	auto noodles = m_ports[index];
	
	for (auto it = noodles->begin(); it != noodles->end(); ++it)
	{
		(*it)->push(sample);
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
