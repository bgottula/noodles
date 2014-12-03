#include "all.h"

Block::~Block(void)
{
	if (m_name != nullptr)
	{
		free(m_name);
	}
}

void Block::register_port(const char *name, Port *p)
{
	int status;
	char *demangled = abi::__cxa_demangle(typeid(*p).name(),
		nullptr, nullptr, &status);
	assert(status == 0);
	debug("%s.register_port: %s %s @ %p\n",
		this->name(), demangled, name, p);
	free(demangled);
	
	if (any_of(m_ports.cbegin(), m_ports.cend(),
		[&](const NamedPort& np) {
			return (np.port == p);
		}))
	{
		throw DuplicatePortException();
	}
	if (any_of(m_ports.cbegin(), m_ports.cend(),
		[&](const NamedPort& np) {
			return (strcmp(name, np.name) == 0);
		}))
	{
		throw DuplicatePortNameException();
	}
	
	NamedPort np { .name = name, .port = p };
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
			int status;
			char *demangled = abi::__cxa_demangle(typeid(x).name(),
				nullptr, nullptr, &status);
			assert(status == 0);
			
			printf("list_ports: name %s type %s addr %p avail %zu\n",
				np.name, demangled, &x, x.available());
		}
		
		IPort *i = dynamic_cast<IPort *>(p);
		OPort *o = dynamic_cast<OPort *>(p);
		
		if (i != nullptr) printf("is an input port.\n");
		if (o != nullptr) printf("is an output port.\n");
		
		// TODO: how do we do this kind of thing, but generically??
		/*InputPort<int> *ip_int = dynamic_cast<InputPort<int> *>(p);
		if (ip_int != nullptr) printf("ip_int = %p\n", ip_int);
		InputPort<float> *ip_float = dynamic_cast<InputPort<float> *>(p);
		if (ip_float != nullptr) printf("ip_float = %p\n", ip_float);
		OutputPort<int> *op_int = dynamic_cast<OutputPort<int> *>(p);
		if (op_int != nullptr) printf("op_int = %p\n", op_int);
		OutputPort<float> *op_float = dynamic_cast<OutputPort<float> *>(p);
		if (op_float != nullptr) printf("op_float = %p\n", op_float);
		*/
	}
}

const char *Block::name(void) const
{
	/* only do this the first time the function is called for this instance */
	if (m_name == nullptr)
	{
		int status;
		m_name = abi::__cxa_demangle(typeid(*this).name(),
			nullptr, nullptr, &status);
		assert(status == 0);
	}
	
	return m_name;
}
