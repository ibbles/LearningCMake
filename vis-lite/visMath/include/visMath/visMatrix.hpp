#include <array>

namespace visMath
{
    struct Matrix
    {
        std::array<float, 16> elements;
    };

    inline Matrix operator+(Matrix const& lhs, Matrix const& rhs)
    {
        Matrix result {};
        for (int i = 0; i < 16; ++i)
        {
            result.elements[i] = lhs.elements[i] + rhs.elements[i];
        }
        return result;
    }

    inline Matrix operator-(Matrix const& lhs, Matrix const& rhs)
    {
        Matrix result {};
        for (int i = 0; i < 16; ++i)
        {
            result.elements[i] = lhs.elements[i] - rhs.elements[i];
        }
        return result;
    }
}