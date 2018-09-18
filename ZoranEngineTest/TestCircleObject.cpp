#include "TestCircleObject.h"

#include <Core/ZoranEngine.h>
#include <Rendering/TextureBase.h>
#include <Physics/PhysicsObjectBase.h>

#include <Core/2D/Components/CircleCollisionComponent.h>
#include <Core/2D/Components/RigidBody2DComponent.h>

#include <Utils/Random.h>

TestCircleObject::TestCircleObject(std::string name, float radius) : TexturedSprite(100, name)
{
	willEverTick = true;

	CircleCollisionComponent* circleComponent = new CircleCollisionComponent(root2DComponent);

	circleComponent->SetRadius(1.0);

	root2DComponent->AddSubComponent(circleComponent);

	SetTexture("circle.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE);
	
	rigidBody = new RigidBody2DComponent(root2DComponent);

	rigidBody->SetRestitution(1.0);
	rigidBody->SetMass(200);

	root2DComponent->AddSubComponent(rigidBody);

	circleComponent->SetPhysicsObjectFrom2DRigidBody(rigidBody);
}


TestCircleObject::~TestCircleObject()
{
}

void TestCircleObject::Tick(float dt)
{
	if (target)
	{
		Vec2D direction = (target->GetPosition() - GetPosition()).getNormal();
		rigidBody->SetGravity(direction * 300);

		if (Random::GetBoolWithChance(0.01f))
		{
			rigidBody->ApplyForce(Vec2D(Random::GetFloatInRange(-10000,10000), 1000000));
		}
	}
}
