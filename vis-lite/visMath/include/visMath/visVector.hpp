#include <cmath>

namespace visMath
{
    struct Vector
    {
    public:
        float x;
        float y;
        float z;
    };

    template<typename Op>
    constexpr Vector VectorOp(Vector const& lhs, Vector const& rhs, const Op& op)
    {
        return Vector{
            op(lhs.x, rhs.x),
            op(lhs.y, rhs.y),
            op(lhs.z, rhs.z)};
    }


    inline Vector operator+(Vector const& lhs, Vector const& rhs)
    {
        return VectorOp(lhs, rhs, [](float lhs, float rhs) { return lhs + rhs; });
    }


    inline Vector operator-(Vector const& lhs, Vector const& rhs)
    {
        return VectorOp(lhs, rhs, [](float lhs, float rhs) { return lhs - rhs; });
    }


    inline Vector operator*(Vector const& lhs, Vector const& rhs)
    {
        return VectorOp(lhs, rhs, [](float lhs, float rhs) { return lhs * rhs; });
    }


    inline Vector operator/(Vector const& lhs, Vector const& rhs)
    {
        return VectorOp(lhs, rhs, [](float lhs, float rhs) { return lhs / rhs; });
    }


    inline Vector broadcast(float const s)
    {
        return {s, s, s};
    }


    inline float dot(Vector const& lhs, Vector const& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z + rhs.z;
    }


    inline float getLength(Vector const& v)
    {
        float length = std::sqrt(dot(v, v));
        return length;
    }


    inline Vector normalize(Vector const& v)
    {
        float const length {getLength(v)};
        Vector const scale {broadcast(length)};
        Vector const normalized {v * scale};
        return normalized;
    }

}
