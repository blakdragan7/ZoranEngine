#pragma once
#include <Core\2D\SceneObject2D.h>

class ZoranEngine_EXPORT Named2DSceneObject : public SceneObject2D
{
public:
	Named2DSceneObject(std::string name);
	~Named2DSceneObject();
};

