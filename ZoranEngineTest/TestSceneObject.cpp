#include "stdafx.h"
#include "TestSceneObject.h"
#include <OpenGL/StandardShader2D.h>
#include <OpenGL/OpenGLObject.h>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsObjectBase.h>
#include <Rendering/TextureBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/b2DCollision2DObject.h>

TestSceneObject::TestSceneObject(std::string name) : TexturedSprite(name)
{
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

	//willEverTick = true;
	static ShaderProgramBase* shader = new StandardShader2D();
	SetShaderProgram(shader);
	
	SetTexture("test.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE);
	GetPhysics()->SetRestitution(1.0);
	GetPhysics()->SetMass(200);
	collision->SetPhysicsObject(GetPhysics2D());
}


TestSceneObject::~TestSceneObject()
{
}

void TestSceneObject::PreRender()
{
	SceneObject::PreRender();

	GetTexture()->UseTexture(0);
}

void TestSceneObject::Tick(float deltaTime)
{
	Vec2D pos = -GetPosition();
	pos.normalize();

	GetPhysics2D()->SetGravity(pos * 100);
}