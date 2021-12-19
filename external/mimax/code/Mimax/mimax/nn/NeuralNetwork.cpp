#include "Mimax_PCH.h"
#include "mimax/nn/NeuralNetwork.h"

#include "mimax/nn/Layer.h"

namespace mimax {
namespace nn {

using namespace std;
using mimax::common::CMatrix;

CNeuralNetwork::~CNeuralNetwork()
{}

CNeuralNetwork::CNeuralNetwork(vector<unique_ptr<ILayer>>&& layers)
    : m_layers(move(layers))
{
}

CMatrix CNeuralNetwork::Predict(CMatrix const& input)
{
    m_buffers[0] = input;

    size_t inputIndex = 0;
    size_t outputIndex = 1;

    for (auto& layer : m_layers)
    {
        layer->Activate(m_buffers[inputIndex], m_buffers[outputIndex]);
        swap(inputIndex, outputIndex);
    }

    return m_buffers[inputIndex];
}

} // nn
} // mimax