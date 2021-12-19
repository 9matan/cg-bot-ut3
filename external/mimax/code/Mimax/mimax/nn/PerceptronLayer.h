#pragma once
#include "mimax/nn/Layer.h"

#include <memory>

namespace mimax {
namespace nn {

class IActivation;

class CPerceptronLayer : public ILayer
{
public:
    ~CPerceptronLayer() override;
    CPerceptronLayer(CPerceptronLayer&& other) = default;
    CPerceptronLayer(std::unique_ptr<IActivation> activation, mimax::common::CMatrix const& weights, mimax::common::CMatrix const& biases);

    void Activate(mimax::common::CMatrix const& input, mimax::common::CMatrix& output) override;

private:
    std::unique_ptr<IActivation> m_activation;
    mimax::common::CMatrix m_weights;
    mimax::common::CMatrix m_biases;
};

} // nn
} // mimax