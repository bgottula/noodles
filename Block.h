#ifndef BLOCK_H
#define BLOCK_H

#include <queue>
#include <cassert>

class Block
{
    public:
        Block(int numInputs = 1, int numOutputs = 1);
        void reset(void);
        virtual void work(void) = 0;
        void pushInput(int input, int inputIndex = 0);
        bool outputEmpty(int outputIndex = 0);
        int popOutput(int outputIndex = 0);

    protected:
        std::queue<int> *m_input;
        const int m_numInputs;
        std::queue<int> *m_output;
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

#endif
