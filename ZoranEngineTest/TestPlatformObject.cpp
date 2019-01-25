#include "TestPlatformObject.h"
#include <Core/2D/Components/OSquareCollisionComponent.h>
#include <Core/2D/Components/AASquareCollisionComponent.h>
#include <Core/2D/Components/RigidBody2DComponent.h>

#include <Physics/Collision/CollisionObjectBase.h>

TestPlatformObject::TestPlatformObject(std::string name) : TexturedSprite(10, name, "test")
{
	//collision = root2DComponent->AddSubComponent<OSquareCollisionComponent>(root2DComponent);
	collision = root2DComponent->AddSubComponent<AASquareCollisionComponent>(root2DComponent);

	collision->SetDynamics(CD_Static);
	collision->SetCollisionLayer(COLLISION_LAYER_GROUND);

	rigid = root2DComponent->AddSubComponent<RigidBody2DComponent>(root2DComponent);

	rigid->SetMass(FLT_MAX); // Static object has infinite mass 
	collision->SetPhysicsObjectFrom2DRigidBody(rigid);
}

TestPlatformObject::~TestPlatformObject()
{
}

