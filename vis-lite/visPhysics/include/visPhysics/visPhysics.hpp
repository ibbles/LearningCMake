#include <visMath/visMath.hpp>

namespace visPhysics
{
    class RigidBody
    {
    public:
        RigidBody();

        visMath::Vec3 getPosition() const;
    private:
        visMath::Vec3 m_position;
    };
}