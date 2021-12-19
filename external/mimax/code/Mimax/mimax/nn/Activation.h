#pragma once
#include "mimax/common/Matrix.h"

namespace mimax {
namespace nn {

class IActivation
{
public:
    virtual ~IActivation() {}
    virtual void Activate(mimax::common::CMatrix& matrix) = 0;
};

} // nn
} // mimax