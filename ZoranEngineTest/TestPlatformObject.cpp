#include "TestPlatformObject.h"
#include <OpenGL/OpenGLObject.h>
#include <OpenGL/StandardShader2D.h>
#include <Rendering/TextureBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>

static StandardShader2D* shader = 0;
TestPlatformObject::TestPlatformObject(std::string name) : TexturedSprite(name, "test.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE)
{
	if (shader == 0)shader = new StandardShader2D();
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
