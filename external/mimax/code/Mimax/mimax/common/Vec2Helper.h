#pragma once

#include "Vec2.h"

#include <istream>
#include <ostream>

namespace mimax {
namespace common {

template <typename TVec>
inline int GetDistanceL1(TVec const lhs, TVec const rhs) { return abs(static_cast<int>(lhs[0] - rhs[0])) + abs(static_cast<int>(lhs[1] - rhs[1])); }

char const* ToString(SVec2i const vec2);
char const* ToString(SVec2 const vec2);

std::ostream& operator<<(std::ostream& out, SVec2char const& vec);
std::ostream& operator<<(std::ostream& out, SVec2char const& vec);

}
}