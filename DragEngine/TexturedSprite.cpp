#include "stdafx.h"
#include "TexturedSprite.h"

#include "TextureBase.h"
#include "RenderEngineBase.h"
#include "DragEngine.h"
#include "PhysicsEngine.h"
#include "BoxCollisionObject.h"
#include "TextureManager.h"

TexturedSprite::TexturedSprite(std::string name) : SceneObject(name)
{
	texture = 0;
	collision = new BoxCollisionObject(Vec3D(0, 0, 0), Vec3D(2, 2, 1), this);
	collision->SetPhysicsObject(GetPhysics());
}

TexturedSprite::TexturedSprite(std::string name, const char* texture, RenderDataType type, RenderDataFormat format) : SceneObject(name)
{
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);
	collision = new BoxCollisionObject(Vec3D(0, 0, 0), Vec3D(2, 2, 1), this);
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