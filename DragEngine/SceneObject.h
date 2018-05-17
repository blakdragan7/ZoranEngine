#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

class SceneObject
{	
public:
	virtual void PostRender() = 0;
	virtual void RenderScene() = 0;
	virtual void PreRender() = 0;
};

