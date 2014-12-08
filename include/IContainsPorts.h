#ifndef ICONTAINSPORTS_H
#define ICONTAINSPORTS_H

class IContainsPorts
{
public:
	virtual ~IContainsPorts() {}
	
	virtual vector<NamedPort>::const_iterator ports_cbegin(void)
	{
		return m_ports.cbegin();
	}
	virtual vector<NamedPort>::const_iterator ports_cend(void)
	{
		return m_ports.cend();
	}
	
	virtual vector<NamedPort>::const_iterator ports_begin(void)
	{
		return m_ports.begin();
	}
	virtual vector<NamedPort>::const_iterator ports_end(void)
	{
		return m_ports.end();
	}
	
	virtual Port *find_port(const char *p_name);
	
protected:
	IContainsPorts() {}
	
	virtual void register_port(const char *p_name, Port *p);
	//virtual void list_ports(void);
	
private:
	vector<NamedPort> m_ports;
};

class NonexistentPortException : public runtime_error
{
public: NonexistentPortException(void) :
	runtime_error("Port with that name has not been registered") {}
};
class DuplicatePortException : public runtime_error
{
public: DuplicatePortException(void) :
	runtime_error("Port instances may only be registered once") {}
};
class DuplicatePortNameException : public runtime_error
{
public: DuplicatePortNameException(void) :
	runtime_error("Port instances must be registered with unique names") {}
};

#define REGISTER_PORT(_p) register_port(#_p, &_p)

#endif
