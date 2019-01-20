#include "TestCircleObject.h"

#include <Core/ZoranEngine.h>
#include <Rendering/TextureBase.h>
#include <Physics/PhysicsObjectBase.h>

#include <Core/2D/Components/CircleCollisionComponent.h>
#include <Core/2D/Components/RigidBody2DComponent.h>

#include <Utils/Random.h>

TestCircleObject::TestCircleObject(std::string name) : TexturedSprite(100, name)
{
	willEverTick = true;

	CircleCollisionComponent* circleComponent = root2DComponent->AddSubComponent<CircleCollisionComponent>(root2DComponent);

	SetTexture("circle.png");
	
	rigidBody = root2DComponent->AddSubComponent<RigidBody2DComponent>(root2DComponent);

	rigidBody->SetRestitution(1.0);
	rigidBody->SetMass(200);

	circleComponent->SetRadius(1.0);
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
			rigidBody->ApplyForce(Vector2D(Random::GetFloatInRange(-10000,10000), 1000000));
		}
	}
}
