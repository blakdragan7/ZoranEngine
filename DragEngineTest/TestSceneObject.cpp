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
	Vec2L size(800,800);
	texture = dEngine->GetRenderer()->CreateTexture("test.png",RenderDataType::TYPE_RGBA_32,RenderDataFormat::FORMAT_UNSIGNED_BYTE,size);

	Scale(Vector3D(0.5,-0.5,1.0));
}


TestSceneObject::~TestSceneObject()
{
}

void TestSceneObject::PreRender()
{
	SceneObject::PreRender();

	OpenGLTexture* oglt = dynamic_cast<OpenGLTexture*>(texture);

	oglt->BindTexture(0);
}
