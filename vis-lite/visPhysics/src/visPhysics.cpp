#include <visPhysics/visPhysics.hpp>

visPhysics::RigidBody::RigidBody()
    : position {visMath::broadcast(0.0F)}
    , velocity {visMath::broadcast(0.0F)}
    , force {visMath::broadcast(0.0F)}
    , mass  {1.0F}
{
}


void visPhysics::integrate(RigidBody& body, float timestep)
{
    visMath::Vector const dt {visMath::broadcast(timestep)};
    visMath::Vector const mass {visMath::broadcast(body.mass)};
    visMath::Vector const acceleration {body.force / mass};
    body.velocity = body.velocity + dt * acceleration;
    body.position = body.position + dt * body.velocity;
    body.force = visMath::broadcast(0.0F);
}


void visPhysics::applyGravity(RigidBody& body, visMath::Vector const& gravity)
{
    body.force = body.force + visMath::broadcast(body.mass) * gravity;
}