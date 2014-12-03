#include "all.h"

#warning TODO
// TODO: make it so that Block subclasses must register their ports in the
// constructor (string -> Port<T>&); this will make debugging still be possible
// ... hmm, where will they be registering them though?

#if 0
class Source : public Block
{
public:
	Source(void)
	{
		outputs.add("phase");
		reset();
	}
	
	void reset(void)
	{
		m_counter = 0;
	}
	
	void work(void)
	{
		if (outputs.available("phase") >= 1)
		{
			int sample = m_counter++;
			
			outputs.put_one("phase", sample);
			printf("Source(%p) sourced %d\n", this, sample);
		}
	}
	
private:
	int m_counter;
};

class VariableDecimator : public Block
{
public:
	VariableDecimator(void)
	{
		inputs.add("phase");
		inputs.add("feedback");
		outputs.add("phase_corrected");
		reset();
	}
	
	void reset(void)
	{
		//m_counter = 0;
	}
	
	void work(void)
	{
		int phase;
		int feedback;
		
		while (inputs.available("phase") >= 1 &&
			inputs.available("feedback") >= 1 &&
			outputs.available("phase_corrected") >= 1)
		{
			inputs.get_one("phase", &phase);
			inputs.get_one("feedback", &feedback);
			
			/* intentionally truncate to [0, 255] */
			int sum = (phase + feedback) % 256;
			outputs.put_one("phase_corrected", sum);
		}
	}
};

class PhaseErrorDetector : public Block
{
public:
	PhaseErrorDetector(void)
	{
		inputs.add("phase_corrected");
		outputs.add("error");
		reset();
	}
	
	void reset(void)
	{
		
	}
	
	void work(void)
	{
		/* dummy: passthru */
		
		if (inputs.available("phase_corrected") >= 1 &&
			outputs.available("error") >= 1)
		{
			int sample;
			inputs.get_one("phase_corrected", &sample);
			outputs.put_one("error", sample);
		}
	}
};

class LoopFilter : public Block
{
public:
	LoopFilter(void)
	{
		inputs.add("error");
		outputs.add("adjustment");
		reset();
	}
	
	void reset(void)
	{
		m_integrator = 0;
	}
	
	void work(void)
	{
		if (inputs.available("error") >= 1 &&
			outputs.available("adjustment") >= 1)
		{
			int error;
			inputs.get_one("error", &error);
			
			int adjustment = (error * m_p) + m_integrator;
			outputs.put_one("adjustment", adjustment);
			
			m_integrator += (error * m_i);
		}
	}
	
private:
	const int m_p = 1;
	const int m_i = 1;
	
	int m_integrator;
};

class Sink : public Block
{
public:
	Sink(void)
	{
		inputs.add("phase_corrected");
		reset();
	}
	
	void reset(void)
	{
		
	}
	
	void work(void)
	{
		int sample;
		
		if (inputs.available("phase_corrected") >= 1)
		{
			inputs.get_one("phase_corrected", &sample);
			printf("Sink(%p) sank %d\n", this, sample);
		}
	}
};
#endif

class ExampleBlock : public Block
{
public:
	ExampleBlock(void)
	{
		REGISTER_PORT(ip_int);
		REGISTER_PORT(ip_float);
		REGISTER_PORT(op_int);
		REGISTER_PORT(op_float);
		
		//list_ports();
	}
	
	void reset(void)
	{
		
	}
	
	void work(void)
	{
		// work once and then return
		// no need to check input/output availability
	}
	
	bool can_work(void)
	{
		// TODO: check input/output availability HERE
		return true;
	}
	
private:
	InputPort<int> ip_int;
	InputPort<float> ip_float;
	
	OutputPort<int> op_int;
	OutputPort<float> op_float;
};

class MyGraph : public Graph
{
public:
	MyGraph(void)
	{
		REGISTER_BLOCK(eblock1);
		REGISTER_BLOCK(eblock2);
	}
	
private:
	ExampleBlock eblock1;
	ExampleBlock eblock2;
};

/* HOWTO: implement block functions
 * can_work()
 * - if inputs are not available, return false
 * - if outputs are not available, return false
 * - else, return true
 * work():
 * - assume that can_work() has already been found to be true
 * - get input(s)
 * - do processing that needs to be done
 * - put output(s)
 * - ONLY DO ONE 'CYCLE' AND THEN RETURN
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
	
	MyGraph mg;
	
#if 0
	Source source;
	VariableDecimator vd;
	PhaseErrorDetector ped;
	LoopFilter lf;
	Sink sink;
	
	Graph g;
	
	/* do a dump of the graph's init state (0 vertices & 0 edges) */
	g.dumpGraph();
	
	g.addQNoodle(4, {&source, "phase"}, {&vd, "phase"});
	g.addQNoodle(1, {&vd, "phase_corrected"}, {&ped, "phase_corrected"});
	g.addQNoodle(1, {&ped, "error"}, {&lf, "error"});
	g.addRNoodle(0, {&lf, "adjustment"}, {&vd, "feedback"});
	g.addQNoodle(4, {&vd, "phase_corrected"}, {&sink, "phase_corrected"});
	
	RoundRobinScheduler sch(g);
	sch.run();
#endif
	
	return 0;
}
