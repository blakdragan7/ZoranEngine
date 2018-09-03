#include "stdafx.h"
#include "TestSceneObject.h"
#include <OpenGL/StandardShader2D.h>
#include <OpenGL/OpenGLRenderObject.h>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsObjectBase.h>
#include <Rendering/TextureBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/b2DCollision2DObject.h>
#include <Utils/Random.h>

TestSceneObject::TestSceneObject(std::string name) : TexturedSprite(name)
{
	willEverTick = true;

	b2DCollision2DObject* sat = new b2DCollision2DObject(this);
	//SatCollision2DObject* sat = new SatCollision2DObject(this);
	collision = sat;

	static const Vec2D points[4] = {
		Vec2D(-1,-1),
		Vec2D(-1,1),
		Vec2D(1,1),
		Vec2D(1,-1)
	};

	//sat->SetAsSquare(points,size);

	static ShaderProgramBase* shader = new StandardShader2D();
	SetShaderProgram(shader);
	
	SetTexture("test.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE);
	GetPhysics()->SetRestitution(1.0);
	GetPhysics()->SetMass(200);
	collision->SetPhysicsObject(GetPhysics2D());

	target = 0;
}


TestSceneObject::~TestSceneObject()
{
}

void TestSceneObject::Tick(float deltaTime)
{
	if (target)
	{
		Vec2D direction = (target->GetPosition() - GetPosition()).getNormal();
		GetPhysics2D()->SetGravity(direction * 300);

		if (Random::GetBoolWithChance(0.01f))
		{
			GetPhysics2D()->ApplyForce(Vec2D(Random::GetFloatInRange(-10000, 10000), 1000000));
		}
	}
}