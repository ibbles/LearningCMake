#pragma once

namespace visMath
{
    struct Vec3
    {
        float x;
        float y;
        float z;
    };

    Vec3 operator+(Vec3 const& lhs, Vec3 const& rhs);
}


