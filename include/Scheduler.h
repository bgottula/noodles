#ifndef SCHEDULER_H
#define SCHEDULER_H

class Scheduler
{
public:
	virtual ~Scheduler() {}
	
	virtual void run(void) = 0;
	
protected:
	Scheduler(Graph *graph) :
		m_graph(graph) {}
	
	Graph *m_graph;
};

class RoundRobinScheduler : public Scheduler
{
public:
	RoundRobinScheduler(Graph *graph) :
		Scheduler(graph) {}
	
	void run(void);
	
private:
	void run_once(void);
};

class ThreadedScheduler : public Scheduler
{
public:
	ThreadedScheduler(Graph *graph) :
		Scheduler(graph) {}
	
	void run(void);
};

class InteractiveScheduler : public Scheduler
{
public:
	InteractiveScheduler(Graph *graph) :
		Scheduler(graph) {}
	
	void run(void);
};

#endif
