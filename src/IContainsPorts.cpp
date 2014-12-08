#include "all.h"

Port *IContainsPorts::find_port(const char *p_name)
{
	auto it = find_if(m_ports.cbegin(), m_ports.cend(),
		[&](const NamedPort& np) {
			return (strcmp(p_name, np.name) == 0);
		});
	
	if (it == m_ports.cend()) throw NonexistentPortException();
	
	return (*it).port;
}

void IContainsPorts::register_port(const char *p_name, Port *p)
{
	debug(AT_BLD "%s::register_port:" AT_RST " %s %s @ %p\n",
		demangle(typeid(*this).name()), demangle(typeid(*p).name()), p_name, p);
	
	if (any_of(m_ports.cbegin(), m_ports.cend(),
		[&](const NamedPort& np) {
			return (np.port == p);
		}))
	{
		throw DuplicatePortException();
	}
	if (any_of(m_ports.cbegin(), m_ports.cend(),
		[&](const NamedPort& np) {
			return (strcmp(p_name, np.name) == 0);
		}))
	{
		throw DuplicatePortNameException();
	}
	
	p->set_owner(this);
	
	NamedPort np { p_name, p };
	m_ports.push_back(np);
}

#if 0
void IContainsPorts::list_ports()
{
	for (auto it = m_ports.begin(); it != m_ports.end(); ++it)
	{
		NamedPort& np = *it;
		Port *p = np.port;
		
		decltype(*p) x = *p;
		{
			printf(AT_BLD "%s::list_ports:" AT_RST " %s %s @ %p, avail = %zu\n",
				demangle(typeid(*this).name()), demangle(typeid(x).name()),
				np.name, &x, x.avail());
		}
		
		IPort *i = dynamic_cast<IPort *>(p);
		OPort *o = dynamic_cast<OPort *>(p);
		
		if (i != nullptr) printf("is an input port.\n");
		if (o != nullptr) printf("is an output port.\n");
	}
}
#endif
