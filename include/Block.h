#ifndef BLOCK_H
#define BLOCK_H

#if 0
struct Sample
{
	
};
#endif

class Ports
{
public:
	void add(const char *name);
	virtual void connect(const char *name, Noodle *noodle) = 0;
	
protected:
	std::unordered_map<const char *, int> m_names;
	vector<vector<Noodle *> *> m_ports;
};
class Inputs : public Ports
{
public:
	void connect(const char *name, Noodle *noodle);
	bool get(const char *name, int *sample);
};
class Outputs : public Ports
{
public:
	void connect(const char *name, Noodle *noodle);
	void put(const char *name, int sample);
};

class Block
{
public:
	virtual void reset(void) = 0;
	virtual void work(void) = 0;
	const char *name(void);
	
	Inputs inputs;
	Outputs outputs;
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
