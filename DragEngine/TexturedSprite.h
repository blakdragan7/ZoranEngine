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
	TexturedSprite(std::string name);
	TexturedSprite(std::string name, const char* texture, RenderDataType type, RenderDataFormat format);
	virtual ~TexturedSprite();

	void SetTexture(const char* path,RenderDataType type,RenderDataFormat format);
	inline TextureBase* GetTexture() { return texture; }
};

