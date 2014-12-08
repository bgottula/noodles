#ifndef SCHEDULER_H
#define SCHEDULER_H

class Scheduler
{
public:
	virtual ~Scheduler() {}
	
	virtual void run() = 0;
	
protected:
	Scheduler(Graph *graph) :
		m_graph(graph) {}
	
	Graph *m_graph;
};

class RoundRobinScheduler final : public Scheduler
{
public:
	RoundRobinScheduler(Graph *graph) :
		Scheduler(graph) {}
	
	void run();
	
private:
	void run_once();
};

class ThreadedScheduler final : public Scheduler
{
public:
	ThreadedScheduler(Graph *graph) :
		Scheduler(graph) {}
	
	void run();
};

class InteractiveScheduler final : public Scheduler
{
public:
	InteractiveScheduler(Graph *graph) :
		Scheduler(graph) {}
	
	void run();
};

#endif
