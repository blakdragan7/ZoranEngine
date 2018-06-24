#pragma once
#include "Core/SceneObject.h"
#include "Math/Vector2.h"
#include "Rendering/RenderTypes.h"

class TextureBase;
class ZoranEngine_EXPORT TexturedSprite : public SceneObject
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
