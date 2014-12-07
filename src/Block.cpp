#include "all.h"

void Block::check(void) const
{
#warning FINISHME
}

Port *Block::find_port(const char *p_name)
{
	auto it = find_if(m_ports.cbegin(), m_ports.cend(),
		[&](const NamedPort& np) {
			return (strcmp(p_name, np.name) == 0);
		});
	
	if (it == m_ports.cend()) throw BlockNonexistentPortException();
	
	return (*it).port;
}

const char *Block::name(void) const
{
	/* only do this the first time the function is called for this instance */
	if (m_name == nullptr)
	{
		decltype(*this) b = *this;
		m_name = demangle(typeid(b).name());
	}
	
	return m_name;
}

void Block::register_port(const char *p_name, Port *p)
{
	debug(AT_BLD "%s.register_port:" AT_RST " %s %s @ %p\n",
		name(), demangle(typeid(*p).name()), p_name, p);
	
	if (any_of(m_ports.cbegin(), m_ports.cend(),
		[&](const NamedPort& np) {
			return (np.port == p);
		}))
	{
		throw BlockDuplicatePortException();
	}
	if (any_of(m_ports.cbegin(), m_ports.cend(),
		[&](const NamedPort& np) {
			return (strcmp(p_name, np.name) == 0);
		}))
	{
		throw BlockDuplicatePortNameException();
	}
	
	p->set_owner(this);
	
	NamedPort np { .name = p_name, .port = p };
	m_ports.push_back(np);
}

void Block::list_ports(void)
{
	for (auto it = m_ports.begin(); it != m_ports.end(); ++it)
	{
		NamedPort& np = *it;
		Port *p = np.port;
		
		decltype(*p) x = *p;
		{
			printf(AT_BLD "%s.list_ports:" AT_RST " %s %s @ %p, avail = %zu\n",
				name(), demangle(typeid(x).name()), np.name, &x, x.available());
		}
		
		IPort *i = dynamic_cast<IPort *>(p);
		OPort *o = dynamic_cast<OPort *>(p);
		
		if (i != nullptr) printf("is an input port.\n");
		if (o != nullptr) printf("is an output port.\n");
	}
}
