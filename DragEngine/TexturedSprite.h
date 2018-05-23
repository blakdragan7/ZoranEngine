#pragma once
#include "SceneObject.h"
#include "Vector2.h"
#include "RenderTypes.h"

class TextureBase;
class DRAGENGINE_EXPORT TexturedSprite : public SceneObject
{
private:
	TextureBase* texture;

public:
	TexturedSprite();
	TexturedSprite(const char* texture, RenderDataType type, RenderDataFormat format, Vec2L size);
	virtual ~TexturedSprite();

	void SetTexture(const char* path,RenderDataType type,RenderDataFormat format, Vec2L size);
	inline TextureBase* GetTexture() { return texture; }
};

