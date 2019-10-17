#include <visPhysics/visPhysics.hpp>

#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
    visPhysics::RigidBody my_body;
    std::cout << my_body.getPosition().x << '\n';
    return 0;
}
