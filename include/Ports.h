#ifndef PORTS_H
#define PORTS_H

typedef vector<Noodle *> Port;

class Ports
{
	friend class Graph;
	
public:
	/* add an input or output port with the given name */
	void add(const char *name);
	/* associate a named port with a noodle */
	virtual void connect(const char *name, Noodle *noodle) = 0;
	
	virtual size_t please(const char *name) = 0;
	
protected:
	/* get a pointer to the port with the given name */
	Port *find_port(const char *name);
	/* get a pointer to the port with the given name and check connections */
	virtual Port *find_port_check(const char *name) = 0;
	
	/* friendly name -> array index mapping */
	unordered_map<const char *, int> m_names;
	/* array of ports, each possibly having multiple noodle pointers */
	vector<Port> m_ports;
};

class InputPorts : public Ports
{
public:
	/* associate a named port with a noodle */
	void connect(const char *name, Noodle *noodle);
	
	/* return the number of samples waiting in the given input port;
	 * this value is guaranteed not to decrease unless get() is called */
	size_t please(const char *name);
	
	/* get one sample from the input port queue */
	void get_one(const char *name, int *sample);
	/* get multiple samples from the input port queue;
	 * only performs one lock/unlock cycle on the queue mutex */
	void get_multi(const char *name, size_t count, int *samples);
	/* get multiple samples from the input port queue;
	 * only performs one lock/unlock cycle on the queue mutex;
	 * expects the given number of int pointers as variadic arguments */
	void get_vari(const char *name, size_t count, ...);
	
private:
	Port *find_port_check(const char *name);
};

class OutputPorts : public Ports
{
public:
	/* associate a named port with a noodle */
	void connect(const char *name, Noodle *noodle);
	
	/* return the number of samples that could fit in the given output port;
	 * this value is guaranteed not to decrease unless put() is called */
	size_t please(const char *name);
	
	/* put one sample onto the output port queue */
	void put_one(const char *name, int sample);
	/* put multiple samples onto the output port queue;
	 * only performs one lock/unlock cycle on the queue mutex */
	void put_multi(const char *name, size_t count, const int *samples);
	/* put multiple samples onto the output port queue;
	 * only performs one lock/unlock cycle on the queue mutex;
	 * expects the given number of ints as variadic arguments */
	void put_vari(const char *name, size_t count, ...);
	
private:
	Port *find_port_check(const char *name);
};

/* thrown in Ports::add */
class DuplicatePortException : public runtime_error
{
public: DuplicatePortException(void) :
	runtime_error("Port names must be unique") {};
};

/* thrown in Ports::connect, Ports::please, InputPorts::get, OutputPorts::put */
class NonexistentPortException : public runtime_error
{
public: NonexistentPortException(void) :
	runtime_error("Port with that name has not been added") {};
};

/* thrown in InputPorts::connect */
class InputMultipleNoodleException : public runtime_error
{
public: InputMultipleNoodleException(void) :
	runtime_error("Input ports cannot be connected to multiple noodles") {};
};

/* thrown in InputPorts::get_* */
class InputNotConnectedException : public runtime_error
{
public: InputNotConnectedException(void) :
	runtime_error("Input port is not connected to a noodle") {};
};
class InputGetImpossibleException : public runtime_error
{
public: InputGetImpossibleException(void) :
	runtime_error("Input port could not possibly have that many samples "
		"available") {};
};
class InputGetUnsuccessfulException : public runtime_error
{
public: InputGetUnsuccessfulException(void) :
	runtime_error("Input port does not currently have that many samples "
		"available") {};
};

/* thrown in OutputPorts::put_* */
class OutputNotConnectedException : public runtime_error
{
public: OutputNotConnectedException(void) :
	runtime_error("Output port is not connected to any noodles") {};
};
class OutputPutImpossibleException : public runtime_error
{
public: OutputPutImpossibleException(void) :
	runtime_error("Output port could not possibly have space for that many "
		"samples") {};
};
class OutputPutUnsuccessfulException : public runtime_error
{
public: OutputPutUnsuccessfulException(void) :
	runtime_error("Output port does not currently have space for that many "
		"samples") {};
};

#warning TODO
// TODO: need tests for each of the 4 newest exceptions

#endif
