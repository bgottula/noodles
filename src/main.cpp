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
		while (outputs.please("output") >= 1)
		{
			outputs.put_one("output", m_counter++);
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
		
		while (inputs.please("input") >= 1 && outputs.please("output") >= 1)
		{
			inputs.get_one("input", &sample);
			if (m_counter % 2 == 0)
			{
				outputs.put_one("output", sample);
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
		
		while (inputs.please("input") >= 1 && outputs.please("output") >= 2)
		{
			inputs.get_one("input", &sample);
			outputs.put_repeat("output", 2, sample);
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
		
		while (inputs.please("input") >= 1)
		{
			inputs.get_one("input", &sample);
			printf("Sink(%p) got %d\n", this, sample);
		}
	}
};

/* HOWTO: implement block work functions
 * 1. figure out if you have the proper inputs to be able to do work
 * 2. figure out if you could store the output in the output queue
 * 3. if conditions (1) or (2) are false, STOP; don't touch inputs or outputs!
 * 4. get input(s)
 * 5. do whatever processing needs to be done
 * 6. put output(s)
 * 
 * TODO: break the current work() function into smaller pieces (e.g.
 * can_do_work, do_one_work, whatever) so that it's harder to accidentally get 
 * this procedure wrong
 */

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
	Sink sink1;
	Sink sink2;
	
	Graph g;
	
	/* do a dump of the graph's init state (0 vertices & 0 edges) */
	g.dumpGraph();
	
	/*g.addNoodle(8, {&source, "output"}, {&decim, "input"});
	g.addNoodle(8, {&decim, "output"}, {&interp, "input"});
	g.addNoodle(8, {&interp, "output"}, {&sink1, "input"});*/
	g.addNoodle(8, {&source, "output"}, {&sink2, "input"});
	
	for (int i = 0; i < 10; i++)
	{
		g.run();
	}
	
	return 0;
}
