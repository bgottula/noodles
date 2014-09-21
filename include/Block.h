#ifndef BLOCK_H
#define BLOCK_H

class Block
{
    public:
        Block(int numInputs = 1, int numOutputs = 1);
        virtual void reset(void) = 0;
        virtual void work(void) = 0;

    protected:
        const int m_numInputs;
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
