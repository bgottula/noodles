#include "all.h"

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

class VariableRateDecimator : public Block
{
public:
	VariableRateDecimator(void)
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
			int sum = (uint8_t)(phase + feedback);
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
	VariableRateDecimator vrd;
	PhaseErrorDetector ped;
	LoopFilter lf;
	Sink sink;
	
	Graph g;
	
	/* do a dump of the graph's init state (0 vertices & 0 edges) */
	g.dumpGraph();
	
	g.addQNoodle(8, {&source, "phase"}, {&vrd, "phase"});
	g.addQNoodle(1, {&vrd, "phase_corrected"}, {&ped, "phase_corrected"});
	g.addQNoodle(1, {&ped, "error"}, {&lf, "error"});
	g.addRNoodle(0, {&lf, "adjustment"}, {&vrd, "feedback"});
	g.addQNoodle(8, {&vrd, "phase_corrected"}, {&sink, "phase_corrected"});
	
	RoundRobinScheduler sch(g);
	sch.run();
	
	return 0;
}
