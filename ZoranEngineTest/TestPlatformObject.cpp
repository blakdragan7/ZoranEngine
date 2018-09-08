#include "TestPlatformObject.h"
#include <Core/2D/Components/OSquareCollisionComponent.h>
#include <Core/2D/Components/RigidBody2DComponent.h>

#include <Physics/Collision/CollisionObjectBase.h>

TestPlatformObject::TestPlatformObject(std::string name) : TexturedSprite(name, "test.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE)
{
	collision = new OSquareCollisionComponent(root2DComponent);

	collision->SetDynamics(CD_Static);
	collision->SetCollisionLayer(COLLISION_LAYER_GROUND);

	root2DComponent->AddSubComponent(collision);

	rigid = new RigidBody2DComponent(root2DComponent);
	
	rigid->SetMass(FLT_MAX); // Static object has infinite mass 

	root2DComponent->AddSubComponent(rigid);

	collision->SetPhysicsObjectFrom2DRigidBody(rigid);
}

TestPlatformObject::~TestPlatformObject()
{
}

