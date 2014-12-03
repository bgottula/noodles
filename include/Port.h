#ifndef PORT_H
#define PORT_H

/* base for all ports (protected ctor prevents direct instantiation) */
class Port
{
public:
	virtual ~Port() {}
	
	virtual size_t available(void) const = 0;
	
protected:
	Port() {}
};

/* base for InputPort<T> (protected ctor prevents direct instantiation) */
class IPort : public Port
{
public:
	virtual ~IPort() {}
	
protected:
	IPort() {}
};

/* base for OutputPort<T> (protected ctor prevents direct instantiation) */
class OPort : public Port
{
public:
	virtual ~OPort() {}
	
protected:
	OPort() {}
};


/* generic InputPort class */
template <typename T>
class InputPort : public IPort
{
public:
	void connect(Noodle<T> *n);
	
	size_t available(void) const;
	
	void get_one(T& sample);
	void get_arr(size_t count, T *samples);
	void get_var(size_t count, ...);
	
	void peek_one(size_t where, T& sample);
	
	typedef T type;
	
protected:
	Noodle<T> *m_noodle = nullptr;
};

/* generic OutputPort class */
template <typename T>
class OutputPort : public OPort
{
public:
	void connect(Noodle<T> *n);
	
	size_t available(void) const;
	
	void put_one(const T& sample);
	void put_repeat(size_t count, const T& sample);
	void put_arr(size_t count, const T *samples);
	void put_var(size_t count, ...);
	
	typedef T type;
	
protected:
	vector<Noodle<T> *> m_noodles;
};

/* associates a name with a port */
struct NamedPort
{
	const char *name;
	Port *port;
};

class InputMultipleNoodleException : public runtime_error
{
public: InputMultipleNoodleException(void) :
	runtime_error("Input ports cannot be connected to multiple noodles") {};
};
class OutputDuplicateNoodleException : public runtime_error
{
public: OutputDuplicateNoodleException(void) :
	runtime_error("Output port is already connected to this noodle") {};
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

class InputGetImpossibleException : public runtime_error
{
public: InputGetImpossibleException(void) :
	runtime_error("Input port could not possibly have that many samples "
		"available") {};
};
class InputGetUnavailableException : public runtime_error
{
public: InputGetUnavailableException(void) :
	runtime_error("Input port does not currently have that many samples "
		"available") {};
};

class InputPeekImpossibleException : public runtime_error
{
public: InputPeekImpossibleException(void) :
	runtime_error("Input port could not possibly have that many samples "
		"available") {};
};
class InputPeekUnavailableException : public runtime_error
{
public: InputPeekUnavailableException(void) :
	runtime_error("Input port does not currently have that many samples "
		"available") {};
};

class OutputPutImpossibleException : public runtime_error
{
public: OutputPutImpossibleException(void) :
	runtime_error("Output port could not possibly have space for that many "
		"samples") {};
};
class OutputPutUnavailableException : public runtime_error
{
public: OutputPutUnavailableException(void) :
	runtime_error("Output port does not currently have space for that many "
		"samples") {};
};

#define TEMPLATES
#include "../src/Port.cpp"
#undef TEMPLATES

#endif
