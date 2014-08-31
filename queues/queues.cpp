#include <iostream>
#include <list>
#include <queue>
#include <stdexcept>
#include <cassert>

using namespace std;


class Block
{
    public:
        Block(int numInputs = 1, int numOutputs = 1)
            : m_numInputs(numInputs), m_numOutputs(numOutputs)
        {
            cout << "Creating block with " << numInputs << " inputs and "
                << numOutputs << " outputs." << endl;
            if (numInputs == 0 && numOutputs == 0)
            {
                throw range_error("Block must have at least one port");
            }
            if (numInputs < 0 || numOutputs < 0)
            {
                throw range_error("Block can't have negative number of ports");
            }
            m_input = new queue<int>[m_numInputs];
            m_output = new queue<int>[m_numOutputs];
            reset();
        }

        void reset(void)
        {
            for (int i = 0; i < m_numInputs; i++)
            {
                m_input[i] = queue<int>();
            }
            for (int i = 0; i < m_numOutputs; i++)
            {
                m_output[i] = queue<int>();
            }
        }
        
        virtual void work(void) = 0;

        void pushInput(int input, int inputIndex = 0)
        {
            assert(inputIndex >= 0 && inputIndex < m_numInputs);
            m_input[inputIndex].push(input);
        }

        bool outputEmpty(int outputIndex = 0)
        {
            assert(outputIndex >= 0 && outputIndex < m_numOutputs);
            return m_output[outputIndex].empty();
        }

        int popOutput(int outputIndex = 0)
        {
            assert(outputIndex >= 0 && outputIndex < m_numOutputs);
            if (m_output[outputIndex].empty())
            {
                throw runtime_error("Tried to pop from empty output queue!");
                return 0;
            }

            int output = m_output[outputIndex].front();
            m_output[outputIndex].pop();
            return output;
        }

    protected:
        queue<int> *m_input;
        const int m_numInputs;
        queue<int> *m_output;
        const int m_numOutputs;
};

class SourceBlock : public Block
{
    public:
        SourceBlock(int numOutputs = 1, int outputsPerWork = 1)
            : Block(0, numOutputs), m_outputsPerWork(outputsPerWork)
        {
            assert(outputsPerWork > 0);
        }
    protected:
        const int m_outputsPerWork;
};

class SinkBlock : public Block
{
    public:
        SinkBlock(int numInputs = 1)
            : Block(numInputs, 0)
        {}
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

