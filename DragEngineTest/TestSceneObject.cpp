#include "stdafx.h"
#include "TestSceneObject.h"
#include "StandardShader.h"
#include "OpenGLObject.h"
#include "TextureBase.h"
#include "DragEngine.h"
#include "RenderEngineBase.h"
#include "OpenGLTexture.h"

TestSceneObject::TestSceneObject()
{
	SetShaderProgram(new StandardShader());
	OpenGLObject* object = dynamic_cast<OpenGLObject*>(GetRenderedObject());

	if (object)
	{
		object->MakeFullScreenQuad();
	}
	SetTexture("test.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE);

	dEngine->AddSceneObject(this);
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
	Vector3D delta = Vector3D(0,0,0) - GetPosition();
	delta.normalize();

	Translate(delta * 0.3 * deltaTime);
}