#include "visMath/visMath.hpp"


using namespace visMath;

Vec3 operator+(Vec3 const& lhs, Vec3 const& rhs)
{
    return {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z};
}