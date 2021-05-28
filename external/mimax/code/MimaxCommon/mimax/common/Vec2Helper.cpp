#include "MimaxCommon_PCH.h"
#include "Vec2Helper.h"

namespace mimax {
namespace common {

char const* ToString(SVec2i const vec2)
{
    static char buff[32];
    sprintf(buff, "{%i:%i}", vec2[0], vec2[1]);
    return buff;
}

char const* ToString(SVec2char const vec2)
{
    static char buff[32];
    sprintf(buff, "{%i:%i}", (int)vec2[0], (int)vec2[1]);
    return buff;
}

std::ostream& operator<<(std::ostream& out, SVec2char const& vec2)
{
    out << (int)vec2[0] << " " << (int)vec2[1];
    return out;
}

std::istream& operator>>(std::istream& in, SVec2char& vec2)
{
    int v1, v2;
    in >> v1 >> v2;
    vec2[0] = (char)v1;
    vec2[1] = (char)v2;
    return in;
}

}
}