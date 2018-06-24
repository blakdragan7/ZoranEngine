#include "stdafx.h"
#include "TexturedSprite.h"

#include "Rendering/TextureBase.h"
#include "Rendering/RenderEngineBase.h"
#include "ZoranEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Physics/Collision/2D/SquareColisionObject.h"
#include "Rendering/TextureManager.h"

TexturedSprite::TexturedSprite(std::string name) : SceneObject(name)
{
	texture = 0;
	collision = new SquareColisionObject(Vec2D(0, 0), Vec2D(2, 2), this);
	collision->SetPhysicsObject(GetPhysics());
}

TexturedSprite::TexturedSprite(std::string name, const char* texture, RenderDataType type, RenderDataFormat format) : SceneObject(name)
{
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);
	collision = new SquareColisionObject(Vec2D(0, 0), Vec2D(2, 2), this);
	collision->SetPhysicsObject(GetPhysics());
}

TexturedSprite::~TexturedSprite()
{
	if (texture)delete texture;
}

void TexturedSprite::SetTexture(const char* path, RenderDataType type, RenderDataFormat format)
{
	texture = TextureManager::GetInstance()->TextureForFilePath(path, type, format);
}
