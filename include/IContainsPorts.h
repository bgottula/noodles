#ifndef ICONTAINSPORTS_H
#define ICONTAINSPORTS_H

class IContainsPorts
{
public:
	virtual ~IContainsPorts() {}
	
	vector<NamedPort>::iterator ports_begin(void)
	{ return m_ports.begin(); }
	vector<NamedPort>::iterator ports_end(void)
	{ return m_ports.end(); }
	
	vector<NamedPort>::const_iterator ports_cbegin(void) const
	{ return m_ports.cbegin(); }
	vector<NamedPort>::const_iterator ports_cend(void) const
	{ return m_ports.cend(); }
	
	vector<NamedPort>::size_type ports_size(void) const
	{ return m_ports.size(); }
	bool ports_empty(void) const
	{ return m_ports.empty(); }
	
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
