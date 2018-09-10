#pragma once
#include <Core\2D\SceneObject2D.h>
#include <Core/2D/Components/DebugRenderComponent.h>
class DebugSceneObject2D : public SceneObject2D
{
private:
	DebugRenderComponent* component;

public:
	DebugSceneObject2D(std::string name);
	~DebugSceneObject2D();

	void SetColor(const Vec3D& color) { component->SetColor(color); }
	void SetColor(float r, float g, float b) { component->SetColor(r, g, b); }
	const Vec3D& GetColor() { return component->GetColor(); }
};

