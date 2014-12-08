#ifndef ICONTAINSPORTS_H
#define ICONTAINSPORTS_H

class IContainsPorts
{
public:
	virtual ~IContainsPorts() {}
	
	virtual void check() const;
	
	virtual Port *find_port(const char *p_name);
	
protected:
	IContainsPorts() {}
	
	virtual void register_port(const char *p_name, Port *p);
	//virtual void list_ports();
	
private:
	vector<NamedPort> m_ports;
	
	typedef vector<NamedPort>::iterator       v_iter;
	typedef vector<NamedPort>::const_iterator v_citer;
	typedef vector<NamedPort>::size_type      v_size;
	
public:
	v_iter ports_begin() { return m_ports.begin(); }
	v_iter ports_end()   { return m_ports.end(); }
	
	v_citer ports_cbegin() const { return m_ports.cbegin(); }
	v_citer ports_cend()   const { return m_ports.cend(); }
	
	v_size ports_size()   const { return m_ports.size(); }
	bool   ports_empty()  const { return m_ports.empty(); }
};

class NonexistentPortException : public runtime_error
{
public: NonexistentPortException() : runtime_error(
	"Port with that name has not been registered") {}
};
class DuplicatePortException : public runtime_error
{
public: DuplicatePortException() : runtime_error(
	"Port instances may only be registered once") {}
};
class DuplicatePortNameException : public runtime_error
{
public: DuplicatePortNameException() : runtime_error(
	"Port instances must be registered with unique names") {}
};

#define REGISTER_PORT(_p) register_port(#_p, &_p)

#endif
