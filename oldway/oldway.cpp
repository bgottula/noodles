#include <iostream>

using namespace std;

class Source
{
    public:
        Source()
        {
            reset();
        }

        void reset(void)
        {
            m_output = 0;
            m_counter = 0;
        }

        void run(void)
        {
           m_output = m_counter++; 
        }

        int getOutput(void) { return m_output; }

    private:
        int m_output;
        int m_counter;
};


class Process
{
    public:
        Process()
        {
            reset();
        }

        void reset(void)
        {
            m_counter = 0;
            m_input = 0;
            m_output = 0;
            m_outputReady = false;
        }

        void run(void)
        {
            if (m_counter % 2 == 0)
            {
                m_output = 2 * m_input;
                m_outputReady = true;
            }
            else
            {
                m_outputReady = false;
            }
            m_counter++;
        }

        int getOutput(void) { return m_output; }
        bool outputReady(void) { return m_outputReady; }
        void setInput(int input) { m_input = input; }

    private:
        int m_counter;
        int m_input;
        int m_output;
        bool m_outputReady;
};

class Sink
{
    public:
        Sink()
        {
            reset();
        }

        void reset(void)
        {
            m_input = 0;
        }

        void run(void)
        {
           cout << "Sink got " << m_input << endl;
        }

        int setInput(int input) { m_input = input; }

    private:
        int m_input;
};


int main()
{
    Source source;
    Process process;
    Sink sink;

    for (int i = 0; i < 10; i++)
    {
        source.run();
        process.setInput(source.getOutput());
        process.run();
        if (process.outputReady())
        {
            sink.setInput(process.getOutput());
            sink.run();
        }
    }
}

