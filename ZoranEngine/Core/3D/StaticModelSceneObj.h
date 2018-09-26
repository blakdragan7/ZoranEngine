#pragma once
#include <Core\3D\SceneObject3D.h>
class StaticObjComponent;
class ZoranEngine_EXPORT StaticModelSceneObj : public SceneObject3D
{
private:
	StaticObjComponent * model;

public:
	StaticModelSceneObj(std::string name);
	~StaticModelSceneObj();

	void SetMesh(const char* meshPath);
};

