#include "all.h"

Block::Block(int numInputs, int numOutputs)
    : m_numInputs(numInputs), m_numOutputs(numOutputs)
{
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

void Block::reset(void)
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

void Block::pushInput(int input, int inputIndex)
{
    assert(inputIndex >= 0 && inputIndex < m_numInputs);
    m_input[inputIndex].push(input);
}

bool Block::outputEmpty(int outputIndex)
{
    assert(outputIndex >= 0 && outputIndex < m_numOutputs);
    return m_output[outputIndex].empty();
}

int Block::popOutput(int outputIndex)
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
