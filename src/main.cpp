#include "all.h"

class Source : public SourceBlock
{
	public:
		Source(void)
		{
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
				m_output[0].push(m_counter++); 
			}
		}
		
	private:
		int m_counter;
};

class Decimator : public Block
{
	public:
		Decimator(void)
		{
			reset();
		}
		
		void reset(void)
		{
			m_counter = 0;
		}
		
		void work(void)
		{
			while (!m_input[0].empty())
			{
				if (m_counter % 2 == 0)
				{
					m_output[0].push(m_input[0].front());
				}
				m_input[0].pop();
				m_counter++;
			}
		}
		
	private:
		int m_counter;
};

class Interpolator : public Block
{
	public:
		void work(void)
		{
			while (!m_input[0].empty())
			{
				m_output[0].push(m_input[0].front());
				m_output[0].push(m_input[0].front());
				m_input[0].pop();
			}
		}
};

class Sink : public SinkBlock
{
	public:
		void work(void)
		{
			while (!m_input[0].empty())
			{
				cout << "Sink got " << m_input[0].front() << endl;
				m_input[0].pop();
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
	
	//Noodle n0(&source, &decim);
	//Noodle n1(&decim, &interp);
	//Noodle n2(&interp, &sink);
	
	/* do a dump of the graph's init state (0 vertices & 0 edges) */
	g.dumpGraph();
	
	//g.addNoodle(&n0);
	//g.addNoodle(&n1);
	//g.addNoodle(&n2);
	
	g.addNoodle(new Noodle(&source, &sink, 0, 0));
	g.addNoodle(new Noodle(&source, &sink, 1, 1));
	g.addNoodle(new Noodle(&source, &sink, 1, 1));
	
	for (int i = 0; i < 10; i++)
	{
		g.run();
	}
	
	return 0;
}
