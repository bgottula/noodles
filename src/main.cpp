#include "all.h"

class Source : public Block
{
public:
	Source(void)
	{
		REGISTER_PORT(o_phase);
		
		reset();
	}
	
	void reset(void)
	{
		m_counter = 0;
	}
	
	bool can_work(void)
	{
		return (o_phase.available() >= 1);
	}
	
	void work(void)
	{
		int sample = m_counter++;
		
		o_phase.put_one(sample);
		printf("Source(%p) sourced %d\n", this, sample);
	}
	
private:
	int m_counter;
	
	OutputPort<int> o_phase;
};

class VariableDecimator : public Block
{
public:
	VariableDecimator(void)
	{
		REGISTER_PORT(i_phase);
		REGISTER_PORT(i_feedback);
		REGISTER_PORT(o_phase_corrected);
		
		reset();
	}
	
	void reset(void) {}
	
	bool can_work(void)
	{
		return (i_phase.available() >= 1 && i_feedback.available() >= 1 &&
			o_phase_corrected.available() >= 1);
	}
	
	void work(void)
	{
		int s_phase;
		int s_feedback;
		
		i_phase.get_one(s_phase);
		i_feedback.get_one(s_feedback);
		
		/* intentionally truncate to [0, 255] */
		int s_sum = (s_phase + s_feedback) % 256;
		o_phase_corrected.put_one(s_sum);
	}
	
private:
	InputPort<int>  i_phase;
	InputPort<int>  i_feedback;
	OutputPort<int> o_phase_corrected;
};

class PhaseErrorDetector : public Block
{
public:
	PhaseErrorDetector(void)
	{
		REGISTER_PORT(i_phase_corrected);
		REGISTER_PORT(o_error);
		
		reset();
	}
	
	void reset(void) {}
	
	bool can_work(void)
	{
		return (i_phase_corrected.available() >= 1 && o_error.available() >= 1);
	}
	
	void work(void)
	{
		/* dummy: passthru */
		
		int sample;
		i_phase_corrected.get_one(sample);
		o_error.put_one(sample);
	}
	
private:
	InputPort<int>  i_phase_corrected;
	OutputPort<int> o_error;
};

class LoopFilter : public Block
{
public:
	LoopFilter(void)
	{
		REGISTER_PORT(i_error);
		REGISTER_PORT(o_adjustment);
		
		reset();
	}
	
	void reset(void)
	{
		m_integrator = 0;
	}
	
	bool can_work(void)
	{
		return (i_error.available() >= 1 && o_adjustment.available() >= 1);
	}
	
	void work(void)
	{
		int s_error;
		i_error.get_one(s_error);
		
		int s_adjustment = (s_error * m_p) + m_integrator;
		o_adjustment.put_one(s_adjustment);
		
		m_integrator += (s_error * m_i);
	}
	
private:
	const int m_p = 1;
	const int m_i = 1;
	
	int m_integrator;
	
	InputPort<int>  i_error;
	OutputPort<int> o_adjustment;
};

class Sink : public Block
{
public:
	Sink(void)
	{
		REGISTER_PORT(i_phase_corrected);
		
		reset();
	}
	
	void reset(void) {}
	
	bool can_work(void)
	{
		return (i_phase_corrected.available() >= 1);
	}
	
	void work(void)
	{
		int sample;
		i_phase_corrected.get_one(sample);
		
		printf("Sink(%p) sank %d\n", this, sample);
	}
	
private:
	InputPort<int> i_phase_corrected;
};

class MyGraph : public Graph
{
public:
	MyGraph(void)
	{
		REGISTER_BLOCK(b_src);
		REGISTER_BLOCK(b_vd);
		REGISTER_BLOCK(b_ped);
		REGISTER_BLOCK(b_lf);
		REGISTER_BLOCK(b_sink);
		
		ADD_QNOODLE(int, 4,
			b_src, o_phase,
			b_vd, i_phase);
		ADD_QNOODLE(int, 1,
			b_vd, o_phase_corrected,
			b_ped, i_phase_corrected);
		ADD_QNOODLE(int, 1,
			b_ped, o_error,
			b_lf, i_error);
		ADD_RNOODLE(int, 0,
			b_lf, o_adjustment,
			b_vd, i_feedback);
		ADD_QNOODLE(int, 4,
			b_vd, o_phase_corrected,
			b_sink, i_phase_corrected);
	}
	
private:
	Source             b_src;
	VariableDecimator  b_vd;
	PhaseErrorDetector b_ped;
	LoopFilter         b_lf;
	Sink               b_sink;
};

/* HOWTO: implement blocks
 * constructor
 * - do REGISTER_PORT() on each port
 * function can_work()
 * - if inputs are not available, return false
 * - if outputs are not available, return false
 * - else, return true
 * function work():
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
	
	MyGraph graph;
	RoundRobinScheduler sch(&graph);
	
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
