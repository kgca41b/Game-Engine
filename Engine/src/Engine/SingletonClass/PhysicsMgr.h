#pragma once
#include "DataTypes.h"
#include "DllMacro.h"

using namespace reactphysics3d;

namespace KGCA41B
{
    class WorldRayCallback : public RaycastCallback
    {
    public:

        virtual decimal notifyRaycastHit(const RaycastInfo& info)
        {
            hitpoint = info.worldPoint;
            body = info.body;
            return decimal(0.0);
        }

        Vector3 hitpoint;
        CollisionBody* body; 
    };

    class DLL_API PhysicsMgr
    {
        SINGLETON(PhysicsMgr);
#define PHYSICS KGCA41B::PhysicsMgr::GetInst()
    public:
        bool Init();
        void Update();
        void Release();

    public:
        PhysicsWorld* GetPhysicsWorld() { return physics_world_; }
        WorldRayCallback WorldPicking(const MouseRay& mouse_ray);
        bool ObjectPicking(const MouseRay& mouse_ray, CollisionBody* target);
    public:
        PhysicsCommon physics_common_;

    private:
        PhysicsWorld* physics_world_;
        float accmulator = 0.0f;
    };
}