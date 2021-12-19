#pragma once
#include "mimax/common/Matrix.h"

namespace mimax {
namespace nn {

class ILayer
{
public:
    virtual ~ILayer() {};
    virtual void Activate(mimax::common::CMatrix const& input, mimax::common::CMatrix& output) = 0;
};

} // nn
} // mimax