#pragma once
#include <Core\3D\SceneObject3D.h>

class Named3DSceneObject : public SceneObject3D
{
public:
	Named3DSceneObject(std::string name);
	~Named3DSceneObject();
};

