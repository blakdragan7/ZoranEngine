#pragma once

#include <Core\2D\Components\Visible2DComponent.h>
#include <Math/Vector3.h>
class DebugShader2D;
class DebugRenderComponent : public Visible2DComponent
{
private:
	DebugShader2D * shader;
	Vector3D color;
	
public:
	DebugRenderComponent();
	~DebugRenderComponent();

	inline void SetColor(const Vec3D& color) { this->color = color; }
	inline void SetColor(float r, float g, float b) { color.x = r; color.y = g; color.z = b; }

	inline const Vector3D& GetColor() { return color; }
};

