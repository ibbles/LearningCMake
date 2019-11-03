#include <visMath/visVector.hpp>

namespace visPhysics
{
    struct RigidBody
    {
        visMath::Vector position;
        visMath::Vector velocity;
        visMath::Vector force;

        float mass;

        RigidBody();
    };


    void integrate(RigidBody& body, float timestep);
    void applyGravity(RigidBody& body, visMath::Vector const& gravity);
}