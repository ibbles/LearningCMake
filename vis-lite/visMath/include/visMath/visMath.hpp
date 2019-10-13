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



struct Base {
    virtual void reimplementMe(int a) {}
};
struct Derived : public Base  {
    void reimplementMe(int a) override {}
};