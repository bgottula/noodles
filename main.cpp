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

    s.addBlock(&source);
    s.addBlock(&decim);
    s.addBlock(&interp);
    s.addBlock(&sink);

    for (int i = 0; i < 10; i++)
    {
        s.run();
    }
}

