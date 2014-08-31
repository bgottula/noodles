#include <iostream>
#include <list>

using namespace std;


class Block
{
    public:
        virtual void reset(void) = 0;
        virtual void work(void) = 0;
        virtual void setInput(int input) = 0;
        virtual bool needsInput(void) = 0;
        virtual int getOutput(void) = 0;
        virtual bool outputValid(void) = 0;
};

class SourceBlock : public Block
{
    public:
        void setInput(int input) {}
        bool needsInput(void) { return false; }
};

class SinkBlock : public Block
{
    public:
        int getOutput(void) { return 0; }
        bool outputValid(void) { return false; }
};

class Scheduler
{
    public:
        void addBlock(Block *block)
        {
            m_blocks.push_back(block);
        }

        void run(void)
        {
            for (list<Block *>::iterator it = m_blocks.begin(), 
                prev = m_blocks.end(); it != m_blocks.end(); 
                prev = it, ++it)
            {
                Block *block = *it;
                if (block->needsInput())
                {
                    /* First block should not need input */
                    if (it == m_blocks.begin())
                    {
                        cerr << "Error: First block demands input!" << endl;
                        return;
                    }
                    else
                    {
                        Block *previousBlock = *prev;
                        if (previousBlock->outputValid())
                        {
                            block->setInput(previousBlock->getOutput());
                        }
                        else
                        {
                            /* Can't go further until upstream blocks produce 
                             * more outputs */
                            break;
                        }
                    }
                }
                /* Ready to do work because block either did not need input
                 * or it got the input it needed to do work */
                block->work();
            }
        }

    private:
        list<Block *> m_blocks;
};

class Source : public SourceBlock
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

        void work(void)
        {
            m_output = m_counter++; 
        }

        int getOutput(void) { return m_output; }
        bool outputValid(void) { return true; }

    private:
        int m_output;
        int m_counter;
};


class Process : public Block
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
            m_outputValid = false;
        }

        void work(void)
        {
            if (m_counter % 2 == 0)
            {
                m_output = 2 * m_input;
                m_outputValid = true;
            }
            else
            {
                m_outputValid = false;
            }
            m_counter++;
        }

        void setInput(int input) { m_input = input; }
        bool needsInput(void) { return true; }
        int getOutput(void) { return m_output; }
        bool outputValid(void) { return m_outputValid; }

    private:
        int m_counter;
        int m_input;
        int m_output;
        bool m_outputValid;
};

class Sink : public SinkBlock
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

        void work(void)
        {
           cout << "Sink got " << m_input << endl;
        }

        void setInput(int input) { m_input = input; }
        bool needsInput(void) { return true; }

    private:
        int m_input;
};


int main()
{
    Source source;
    Process process;
    Sink sink;

    Scheduler s;

    s.addBlock(&source);
    s.addBlock(&process);
    s.addBlock(&sink);

    for (int i = 0; i < 10; i++)
    {
        s.run();
    }
}

