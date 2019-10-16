#include <visPhysics/visPhysics.hpp>

visPhysics::RigidBody::RigidBody()
    : m_position {0.0F, 0.0F, 0.0F}
{
    m_position = m_position + visMath::Vec3 {1.0F, 1.0F, 1.0F};
}