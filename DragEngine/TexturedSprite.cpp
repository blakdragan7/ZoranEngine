#include "stdafx.h"
#include "TexturedSprite.h"

#include "TextureBase.h"
#include "RenderEngineBase.h"
#include "DragEngine.h"
#include "PhysicsEngine.h"
#include "BoxCollisionObject.h"
#include "TextureManager.h"

TexturedSprite::TexturedSprite()
{
	texture = 0;
	collision = new BoxCollisionObject(Vec3D(0, 0, 0), Vec3D(2, 2, 1), this);
	collision->SetPhysicsObject(GetPhysics());
	pEngine->AddCollisionObject(collision);
}

TexturedSprite::TexturedSprite(const char* texture, RenderDataType type, RenderDataFormat format) : SceneObject()
{
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);
	collision = new BoxCollisionObject(Vec3D(0,0,0), Vec3D(1,1,1), this);
	pEngine->AddCollisionObject(collision);
}

TexturedSprite::~TexturedSprite()
{
	if (texture)delete texture;
}

void TexturedSprite::SetTexture(const char* path, RenderDataType type, RenderDataFormat format)
{
	texture = TextureManager::GetInstance()->TextureForFilePath(path, type, format);
}