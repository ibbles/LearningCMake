#include "visMath/visVector.hpp"

namespace visMath
{
    inline Vector project(Vector const& v, Vector const& onto)
    {
        Vector const direction {normalize(onto)};
        float const newLength {dot(v, direction)};
        Vector const scale {broadcast(newLength)};
        Vector const result = scale * direction;
        return result;
    }
}