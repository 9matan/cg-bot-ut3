#pragma once
#include <memory>
#include <vector>

#include "mimax/common/Matrix.h"

namespace mimax {
namespace nn {

class ILayer;

class CNeuralNetwork
{
public:
    ~CNeuralNetwork();
    CNeuralNetwork(std::vector<std::unique_ptr<ILayer>>&& layers);

    mimax::common::CMatrix Predict(mimax::common::CMatrix const& input);

private:
    std::vector<std::unique_ptr<ILayer>> m_layers;
    mimax::common::CMatrix m_buffers[2];
};

} // nn
} // mimax