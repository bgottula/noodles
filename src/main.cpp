#include "all.h"

class Source : public Block
{
public:
	Source(void)
	{
		outputs.add("output");
		reset();
	}
	
	void reset(void)
	{
		m_counter = 0;
	}
	
	void work(void)
	{
		for (int i = 0; i < m_outputsPerWork; i++)
		{
			outputs.put("output", m_counter++);
		}
	}
	
private:
	int m_outputsPerWork = 1;
	int m_counter;
};

class Decimator : public Block
{
public:
	Decimator(void)
	{
		inputs.add("input");
		outputs.add("output");
		reset();
	}
	
	void reset(void)
	{
		m_counter = 0;
	}
	
	void work(void)
	{
		int sample;
		
		while (inputs.get("input", &sample))
		{
			if (m_counter % 2 == 0)
			{
				outputs.put("output", sample);
			}
			m_counter++;
		}
	}
	
private:
	int m_counter;
};

class Interpolator : public Block
{
public:
	Interpolator(void)
	{
		inputs.add("input");
		outputs.add("output");
		reset();
	}
	
	void reset(void) {}
	
	void work(void)
	{
		int sample;
		
		while (inputs.get("input", &sample))
		{
			outputs.put("output", sample);
			outputs.put("output", sample);
		}
	}
};

class Sink : public Block
{
public:
	Sink(void)
	{
		inputs.add("input");
		reset();
	}
	
	void reset(void) {}
	
	void work(void)
	{
		int sample;
		
		while (inputs.get("input", &sample))
		{
			printf("Sink got %d\n", sample);
		}
	}
};

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-v") == 0)
		{
			verbose = true;
		}
		else
		{
			fprintf(stderr, "unrecognized command-line param: '%s'\n", argv[i]);
			return 1;
		}
	}
	
	Source source;
	Decimator decim;
	Interpolator interp;
	Sink sink;
	
	Graph g;
	
	/* do a dump of the graph's init state (0 vertices & 0 edges) */
	g.dumpGraph();
	
	g.addNoodle({&source, "output"}, {&decim, "input"});
	g.addNoodle({&decim, "output"}, {&interp, "input"});
	g.addNoodle({&interp, "output"}, {&sink, "input"});
	
	for (int i = 0; i < 10; i++)
	{
		g.run();
	}
	
	return 0;
}
