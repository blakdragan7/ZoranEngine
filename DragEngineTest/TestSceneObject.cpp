#include "stdafx.h"
#include "TestSceneObject.h"
#include "StandardShader.h"
#include "OpenGLObject.h"
#include "TextureBase.h"
#include "DragEngine.h"
#include "RenderEngineBase.h"
#include "OpenGLTexture.h"
#include <PhysicsObjectBase.h>

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
	//RotateByEulor(Vec3D(0, 0, 1.5*deltaTime));
}