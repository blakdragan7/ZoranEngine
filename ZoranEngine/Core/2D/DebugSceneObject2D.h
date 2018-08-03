#pragma once
#include <Core\2D\SceneObject2D.h>

class DebugSceneObject2D : public SceneObject2D
{
private:
	Vec3D Color; // rgb 

public:
	DebugSceneObject2D(std::string name);
	~DebugSceneObject2D();

	void SetColor(Vec3D color) { Color = color; }
	Vec3D GetColor() { return Color; }
};

