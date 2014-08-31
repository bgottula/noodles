#include <iostream>
#include <list>
#include <queue>
#include <stdexcept>

using namespace std;


class Block
{
    public:
        void reset(void)
        {
            m_input = queue<int>();
            m_output = queue<int>();
        }
        
        virtual void work(void) = 0;

        void pushInput(int input)
        {
            m_input.push(input);
        }

        bool outputEmpty(void)
        {
            return m_output.empty();
        }

        int popOutput(void)
        {
            if (m_output.empty())
            {
                throw runtime_error("Tried to pop from empty output queue!");
                return 0;
            }

            int output = m_output.front();
            m_output.pop();
            return output;
        }

    protected:
        queue<int> m_input;
        queue<int> m_output;
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
                /* First block should not need input */
                if (it != m_blocks.begin())
                {
                    /* Move all outputs from the previous block to the input 
                     * of the current block */
                    Block *previousBlock = *prev;
                    while (!previousBlock->outputEmpty())
                    {
                        block->pushInput(previousBlock->popOutput());
                    }
                }
                block->work();
            }
        }

    private:
        list<Block *> m_blocks;
};

class Source : public Block
{
    public:
        Source()
        {
            reset();
        }

        void reset(void)
        {
            Block::reset();
            m_counter = 0;
        }

        void work(void)
        {
            m_output.push(m_counter++); 
        }

    private:
        int m_counter;
};

class Decimator : public Block
{
    public:
        Decimator()
        {
            reset();
        }

        void reset(void)
        {
            Block::reset();
            m_counter = 0;
        }

        void work(void)
        {
            while (!m_input.empty())
            {
                if (m_counter % 2 == 0)
                {
                    m_output.push(m_input.front());
                }
                m_input.pop();
                m_counter++;
            }
        }

    private:
        int m_counter;
};

class Interpolator : public Block
{
    public:
        Interpolator()
        {
            reset();
        }

        void reset(void)
        {
            Block::reset();
        }

        void work(void)
        {
            while (!m_input.empty())
            {
                m_output.push(m_input.front());
                m_output.push(m_input.front());
                m_input.pop();
            }
        }
};

class Sink : public Block
{
    public:
        Sink()
        {
            reset();
        }

        void work(void)
        {
            while (!m_input.empty())
            {
                cout << "Sink got " << m_input.front() << endl;
                m_input.pop();
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

