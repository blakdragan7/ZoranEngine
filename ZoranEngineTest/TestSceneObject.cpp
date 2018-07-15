#include "stdafx.h"
#include "TestSceneObject.h"
#include <OpenGL/StandardShader2D.h>
#include <OpenGL/OpenGLObject.h>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsObjectBase.h>
#include <Rendering/TextureBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
static StandardShader2D* shader = 0;
TestSceneObject::TestSceneObject(std::string name) : TexturedSprite(name)
{
	//willEverTick = true;
	if (shader == 0)shader = new StandardShader2D();
	SetShaderProgram(shader);
	
	SetTexture("test.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE);
	GetPhysics()->SetRestitution(1.0);
	GetPhysics()->SetMass(20);
}


TestSceneObject::~TestSceneObject()
{
}

void TestSceneObject::PreRender()
{
	SceneObject::PreRender();

	GetTexture()->UseTexture(0);
}

void TestSceneObject::Tick(double deltaTime)
{
	Vec2D pos = -GetPosition();
	pos.normalize();

	GetPhysics2D()->SetGravity(pos * 100);
}