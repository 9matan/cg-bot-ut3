#include "Mimax_PCH.h"

#include <algorithm>
#include <limits>

#include "mimax/nn/ActivationReLU.h"

namespace mimax {
namespace nn {

using Scalar = mimax::common::CMatrix::Scalar;

static inline Scalar CActivationReLU_Activate(Scalar const value)
{
    return (value + std::numeric_limits<Scalar>::epsilon()) >= mimax::common::CMatrix::ScalarZero
        ? value
        : mimax::common::CMatrix::ScalarZero;
}

void CActivationReLU::Activate(mimax::common::CMatrix& matrix)
{
    std::transform(matrix.begin(), matrix.end(), matrix.begin(), CActivationReLU_Activate);
}

} // nn
} // mimax