#include "TestPlatformObject.h"
#include <OpenGL/OpenGLObject.h>
#include <Rendering/RenderEngineBase.h>
#include <OpenGL/StandardShader.h>
#include <Rendering/TextureBase.h>
#include <Physics/Collision/CollisionObjectBase.h>

static StandardShader* shader = 0;
TestPlatformObject::TestPlatformObject() : TexturedSprite("Ground", "test.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE)
{
	if (shader == 0)shader = new StandardShader();
	SetShaderProgram(shader);
	
	OpenGLObject* object = dynamic_cast<OpenGLObject*>(GetRenderedObject());

	if (object)
	{
		object->MakeFullScreenQuad();
	}

	collision->SetDynamics(CD_Static);
}

TestPlatformObject::~TestPlatformObject()
{
}

void TestPlatformObject::PreRender()
{
	SceneObject::PreRender();

	GetTexture()->UseTexture(0);
}
