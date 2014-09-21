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

#endif
