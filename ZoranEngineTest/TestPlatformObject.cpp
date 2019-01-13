#include "TestPlatformObject.h"
#include <Core/2D/Components/OSquareCollisionComponent.h>
#include <Core/2D/Components/AASquareCollisionComponent.h>
#include <Core/2D/Components/RigidBody2DComponent.h>

#include <Physics/Collision/CollisionObjectBase.h>

TestPlatformObject::TestPlatformObject(std::string name) : TexturedSprite(10, name, "test.png", RenderDataType::Render_Data_Type_RGBA_32, RenderDataFormat::Render_Data_Format_Unsigned_Byte)
{
	//collision = new OSquareCollisionComponent(root2DComponent);
	collision = new AASquareCollisionComponent(root2DComponent);

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

