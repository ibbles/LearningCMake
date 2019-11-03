#include <visPhysics/visPhysics.hpp>

#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
    // Simulation settings.
    float const timestep {1.0F / 60.0F};
    float const end_time {10.0F};

    // World settings.
    visMath::Vector gravity {0.0F, 0.0F, -9.82F};

    // Scene setup.
    visPhysics::RigidBody body;

    // Main loop.
    float time {0.0F};
    while (time < end_time)
    {
        time += timestep;
        visPhysics::applyGravity(body, gravity);
        visPhysics::integrate(body, timestep);
        std::cout << body.position.z << '\n';
    }
    return 0;
}
