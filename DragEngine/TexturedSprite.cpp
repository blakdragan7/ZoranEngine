#include "stdafx.h"
#include "TexturedSprite.h"

#include "RenderEngineBase.h"
#include "DragEngine.h"

TexturedSprite::TexturedSprite()
{
	texture = 0;
}

TexturedSprite::TexturedSprite(const char* texture, RenderDataType type, RenderDataFormat format, Vec2L size) : SceneObject()
{
	texture = dEngine->GetRenderer()->CreateTexture(texture, type, format, size);
}

TexturedSprite::~TexturedSprite()
{
	if (texture)delete texture;
}

void TexturedSprite::SetTexture(const char* path, RenderDataType type, RenderDataFormat format, Vec2L size)
{
	texture = dEngine->GetRenderer()->CreateTexture(path, type, format, size);
}