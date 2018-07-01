#include "stdafx.h"
#include "TexturedSprite.h"

#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Rendering/TextureBase.h>
#include <Rendering/TextureManager.h>
#include <Rendering/RenderEngineBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/SquareColisionObject.h>

TexturedSprite::TexturedSprite(std::string name) : SceneObject2D(name)
{
	texture = 0;
	physicsObject = new PhysicsObject2DBase(this);
	collision = new SquareColisionObject(Vec2D(0, 0), Vec2D(2, 2), this);
	collision->SetPhysicsObject(GetPhysics2D());
}

TexturedSprite::TexturedSprite(std::string name, const char* texture, RenderDataType type, RenderDataFormat format) : SceneObject2D(name)
{
	physicsObject = new PhysicsObject2DBase(this);
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);
	collision = new SquareColisionObject(Vec2D(0, 0), Vec2D(2, 2), this);
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
