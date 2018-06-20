#include "stdafx.h"
#include "TexturedSprite.h"

#include "TextureBase.h"
#include "RenderEngineBase.h"
#include "DragEngine.h"
#include "PhysicsEngine.h"
#include "SquareColisionObject.h"
#include "TextureManager.h"

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