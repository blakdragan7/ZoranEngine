#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/Primitives.h>
class ShaderProgramBase;
class ZoranEngine_EXPORT GUIRenderer : public RenderedObjectBase
{
protected:
	Color tint;
	bool hasTexture;

public:
	GUIRenderer();
	~GUIRenderer();

	virtual ShaderProgramBase* GetShaderProgram()const = 0;

	inline void SetTint(const Color& t) { this->tint = t; }
	inline const Color& GetTint() { return this->tint; }

	inline void SetHasTexture(bool hasTexture) { this->hasTexture = hasTexture; }
	inline bool GetHasTexture() { return hasTexture; }
};

