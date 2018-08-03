#include "TestPlatformObject.h"
#include <OpenGL/OpenGLObject.h>
#include <OpenGL/StandardShader2D.h>
#include <Rendering/TextureBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>

TestPlatformObject::TestPlatformObject(std::string name) : TexturedSprite(name, "test.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE)
{
	collision = new AABBSquareCollisionObject(Vec2D(0, 0), Vec2D(2, 2), this);
	collision->SetPhysicsObject(GetPhysics2D());

	static ShaderProgramBase* shader = new StandardShader2D();
	SetShaderProgram(shader);
	
	OpenGLObject* object = dynamic_cast<OpenGLObject*>(GetRenderedObject());

	if (object)
	{
		object->MakeFullScreenQuad();
	}

	collision->SetDynamics(CD_Static);
	collision->SetCollisionLayer(COLLISION_LAYER_GROUND);

	GetPhysics2D()->SetMass(FLT_MAX); // Static object has infinite mass 
}

TestPlatformObject::~TestPlatformObject()
{
}

void TestPlatformObject::PreRender()
{
	SceneObject::PreRender();

	GetTexture()->UseTexture(0);
}
