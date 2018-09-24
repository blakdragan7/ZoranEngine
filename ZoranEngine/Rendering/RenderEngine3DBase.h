#pragma once
#include <Rendering\RenderEngineBase.h>

class Visible3DComponent;
class ZoranEngine_EXPORT RenderEngine3DBase : public RenderEngineBase
{
public:
	RenderEngine3DBase();
	virtual ~RenderEngine3DBase();


	/* Each implementation must implement way of adding Render Objects
	*  ex. DirectX may have a different render loop style then OpenGL */
	virtual void AddComponent(Visible3DComponent* component) = 0;
	virtual bool RemoveComponent(Visible3DComponent* component) = 0;
};

