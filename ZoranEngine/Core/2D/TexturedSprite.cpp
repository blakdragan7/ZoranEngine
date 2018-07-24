#include "stdafx.h"
#include "TexturedSprite.h"

#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Rendering/TextureBase.h>
#include <Rendering/TextureManager.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/RenderedObjectBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Physics/2D/Collision/b2DCollision2DObject.h>
#include <Physics/2D/Collision/SatCollision2DObject.h>

TexturedSprite::TexturedSprite(std::string name) : SceneObject2D(name)
{
	texture = 0;
	physicsObject = new PhysicsObject2DBase(this);
	//b2DCollision2DObject* sat = new b2DCollision2DObject(this);
	SatCollision2DObject* sat = new SatCollision2DObject(this);
	collision = sat;
	startingSize = Vec2D(2,2);
	SetScale(1,1);

	//collision = new AABBSquareCollisionObject(Vec2D(0, 0), Vec2D(2, 2), this);
	renderedObject->MakeFullScreenQuad();
	collision->SetPhysicsObject(GetPhysics2D());

	static const Vec2D points[4] = {
		Vec2D(-1,-1),
		Vec2D(-1,1),
		Vec2D(1,1),
		Vec2D(1,-1)
	};

	sat->SetAsSquare(points,size);
}

TexturedSprite::TexturedSprite(std::string name, const char* texture, RenderDataType type, RenderDataFormat format) : SceneObject2D(name)
{
	startingSize = Vec2D(2, 2);
	SetScale(1,1);
	physicsObject = new PhysicsObject2DBase(this);
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);
	collision = new AABBSquareCollisionObject(Vec2D(0, 0), Vec2D(2, 2), this);
	collision->SetPhysicsObject(GetPhysics2D());
}

TexturedSprite::~TexturedSprite()
{
	if (texture)delete texture;
}

void TexturedSprite::SetTexture(const char* path, RenderDataType type, RenderDataFormat format)
{
	texture = TextureManager::GetInstance()->TextureForFilePath(path, type, format);
}
