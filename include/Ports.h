#ifndef PORTS_H
#define PORTS_H

class Ports
{
	friend class Graph;
	
public:
	void add(const char *name);
	virtual void connect(const char *name, Noodle *noodle) = 0;
	
protected:
	unordered_map<const char *, int> m_names;
	vector<vector<Noodle *> > m_ports;
};

class InputPorts : public Ports
{
public:
	void connect(const char *name, Noodle *noodle);
	bool get(const char *name, int *sample);
};

class OutputPorts : public Ports
{
public:
	void connect(const char *name, Noodle *noodle);
	void put(const char *name, int sample);
};

class DuplicatePortException : public runtime_error
{
public: DuplicatePortException(void) :
	runtime_error("Port names must be unique") {};
};
class NonexistentPortException : public runtime_error
{
public: NonexistentPortException(void) :
	runtime_error("Port with that name has not been added") {};
};
class InputMultipleNoodleException : public runtime_error
{
public: InputMultipleNoodleException(void) :
	runtime_error("Input ports cannot be connected to multiple noodles") {};
};
class InputNotConnectedException : public runtime_error
{
public: InputNotConnectedException(void) :
	runtime_error("Input port is not connected to a noodle") {};
};
class OutputNotConnectedException : public runtime_error
{
public: OutputNotConnectedException(void) :
	runtime_error("Output port is not connected to any noodles") {};
};

#endif
