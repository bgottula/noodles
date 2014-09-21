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
}
