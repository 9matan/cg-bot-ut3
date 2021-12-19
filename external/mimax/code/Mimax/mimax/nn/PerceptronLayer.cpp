#include "Mimax_PCH.h"
#include "mimax/nn/PerceptronLayer.h"

#include "mimax/nn/Activation.h"

namespace mimax {
namespace nn {

using namespace std;
using mimax::common::CMatrix;

CPerceptronLayer::CPerceptronLayer(unique_ptr<IActivation> activation, CMatrix const& weights, CMatrix const& biases)
    : m_activation(move(activation))
    , m_weights(weights)
    , m_biases(biases)
{
    assert(!m_weights.empty());
    assert(!m_biases.empty());
    assert(m_weights.GetColsCount() == m_biases.GetColsCount());
    assert(CMatrix::IsRowVector(m_biases));
}

CPerceptronLayer::~CPerceptronLayer()
{}

void CPerceptronLayer::Activate(CMatrix const& input, CMatrix& output)
{
    assert(!input.empty());
    assert(input.GetColsCount() == m_weights.GetRowsCount());

    output = input * m_weights;
    output.AddToEachRowInPlace(m_biases);
    m_activation->Activate(output);
}

} // nn
} // mimax