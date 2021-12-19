#pragma once
#include "mimax/nn/Activation.h"

namespace mimax {
namespace nn {

class CActivationReLU : public IActivation
{
public:
    void Activate(mimax::common::CMatrix& matrix) override;
};

} // nn
} // mimax