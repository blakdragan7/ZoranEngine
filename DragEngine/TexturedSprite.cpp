#include "stdafx.h"
#include "TexturedSprite.h"

#include "TextureBase.h"
#include "RenderEngineBase.h"
#include "DragEngine.h"
#include "PhysicsEngine.h"
#include "BoxCollisionObject.h"

TexturedSprite::TexturedSprite()
{
	texture = 0;
	collision = new BoxCollisionObject(Vec3D(0, 0, 0), Vec3D(1, 1, 1), this);
	pEngine->AddCollisionObject(collision);
}

TexturedSprite::TexturedSprite(const char* texture, RenderDataType type, RenderDataFormat format, Vec2L size) : SceneObject()
{
	this->texture = dEngine->GetRenderer()->CreateTexture(texture, type, format, size);
	collision = new BoxCollisionObject(Vec3D(0,0,0), Vec3D(1,1,1), this);
	pEngine->AddCollisionObject(collision);
}

TexturedSprite::~TexturedSprite()
{
	if (texture)delete texture;
	pEngine->RemoveObject(collision);
	delete collision;
}

void TexturedSprite::SetTexture(const char* path, RenderDataType type, RenderDataFormat format, Vec2L size)
{
	texture = dEngine->GetRenderer()->CreateTexture(path, type, format, size);
}