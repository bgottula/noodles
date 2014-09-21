#include "all.h"

void Ports::add(const char *name)
{
	int index = m_noodles.size();
	
	// TODO: prevent duplicate names
	
	m_names.insert({name, index});
	m_noodles.push_back(nullptr);
}

void Ports::connect(const char *name, Noodle *noodle)
{
	// TODO: ensure that an input/output with this name exists
	int index = m_names[name];
	
	// TODO: ensure that the index isn't out of bounds
	m_noodles[index] = noodle;
	
	// TODO: ensure that this port isn't already connected (for inputs only)
}

bool Inputs::get(const char *name, int *sample)
{
	// TODO: ensure that an input with this name exists
	int index = m_names[name];
	
	if (m_noodles[index]->empty())
	{
		return false;
	}
	else
	{
		*sample = m_noodles[index]->pop();
		return true;
	}
}

void Outputs::put(const char *name, int sample)
{
	// TODO: ensure that an output with this name exists
	int index = m_names[name];
	
	m_noodles[index]->push(sample);
}
