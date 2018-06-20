#include "stdafx.h"
#include "TestSceneObject.h"
#include <StandardShader.h>
#include <OpenGLObject.h>
#include <DragEngine.h>
#include <PhysicsObjectBase.h>
#include <TextureBase.h>

static StandardShader* shader = 0;
TestSceneObject::TestSceneObject(std::string name) : TexturedSprite(name)
{
	if (shader == 0)shader = new StandardShader();
	SetShaderProgram(shader);
	OpenGLObject* object = dynamic_cast<OpenGLObject*>(GetRenderedObject());

	if (object)
	{
		object->MakeFullScreenQuad();
	}
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

}