#include "stdafx.h"
#include "TexturedSprite.h"

#include "RenderEngineBase.h"
#include "DragEngine.h"

TexturedSprite::TexturedSprite()
{
	texture = 0;
}

TexturedSprite::TexturedSprite(const char* texture) : SceneObject()
{
	if (texture)delete texture;
}

TexturedSprite::~TexturedSprite()
{
}

void TexturedSprite::SetTexture(const char* path, RenderDataType type, RenderDataFormat format, Vec2L size)
{
	texture = dEngine->GetRenderer()->CreateTexture(path, type, format, size);
}