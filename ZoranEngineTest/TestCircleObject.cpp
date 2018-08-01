#include "TestCircleObject.h"
#include <OpenGL/StandardShader2D.h>
#include <OpenGL/OpenGLObject.h>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsObjectBase.h>
#include <Rendering/TextureBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/CircleCollision2DObject.h>

TestCircleObject::TestCircleObject(std::string name, float radius) : TexturedSprite(name)
{
	collision = new CircleCollision2DObject(1.0, this, CD_Dynamic);
	collision->SetPhysicsObject(GetPhysics2D());

	static ShaderProgramBase* shader = new StandardShader2D();
	SetShaderProgram(shader);

	SetTexture("circle.png", RenderDataType::TYPE_RGBA_32, RenderDataFormat::FORMAT_UNSIGNED_BYTE);
	GetPhysics()->SetRestitution(1.0);
	GetPhysics()->SetMass(200);
}


TestCircleObject::~TestCircleObject()
{
}
