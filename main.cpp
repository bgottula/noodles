#include <iostream>
#include "Block.h"
#include "Scheduler.h"

using namespace std;

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


int main()
{
    Source source;
    Decimator decim;
    Interpolator interp;
    Sink sink;

    Scheduler s;

    /* use bogus indices here just so we can see them in the graph dump */
    Connection c0(&source, &decim, 1, 2);
    Connection c1(&decim, &interp, 3, 4);
    Connection c2(&interp, &sink, 5, 6);
    Connection c3(&source, &sink, 1, 6);
    
    /* do a dump of the graph's init state (0 vertices & 0 edges) */
    s.dumpGraph();

    s.addConnection(&c0);
    s.addConnection(&c1);
    s.addConnection(&c2);
    
    /* intentionally try to add a duplicate edge to see if it gets caught */
    s.addConnection(&c2);
    
    /* test the graph checking code */
    s.addConnection(&c3);
    
    for (int i = 0; i < 10; i++)
    {
        s.run();
    }
}

