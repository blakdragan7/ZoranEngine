#include "stdafx.h"
#include "TestSceneObject.h"
#include <Core/ZoranEngine.h>
#include <Core/2D/Components/OSquareCollisionComponent.h>
#include <Core/2D/Components/RigidBody2DComponent.h>
#include <Rendering/RenderEngineBase.h>
#include <Physics/PhysicsEngine.h>
#include <Utils/Random.h>

TestSceneObject::TestSceneObject(std::string name) : TexturedSprite(100, name)
{
	willEverTick = true;

	collision = root2DComponent->AddSubComponent<OSquareCollisionComponent>(root2DComponent);

	collision->SetStartingSize(2, 2);
	collision->SetScale(1, 1);
	
	rigid = root2DComponent->AddSubComponent<RigidBody2DComponent>(root2DComponent);

	rigid->SetMass(200);
	rigid->SetRestitution(1.0f);

	collision->SetPhysicsObjectFrom2DRigidBody(rigid);

	target = 0;

	SetTexture("test");
}


TestSceneObject::~TestSceneObject()
{
}

void TestSceneObject::Tick(float deltaTime)
{
	if (target)
	{
		Vec2D direction = (target->GetPosition() - GetPosition()).getNormal();
		rigid->SetGravity(direction * 300);

		if (Random::GetBoolWithChance(0.01f))
		{
			rigid->ApplyForce(Vector2D(Random::GetFloatInRange(-10000, 10000), 1000000));
		}
	}
}