#pragma once
#include <Rendering\RenderEngineBase.h>

class Visible2DComponent;
class ZoranEngine_EXPORT RenderEngine2DBase : public RenderEngineBase
{
public:
	RenderEngine2DBase();
	virtual ~RenderEngine2DBase();


	/* Each implementation must implement way of adding Render Objects
	*  ex. DirectX may have a different render loop style then OpenGL */
	virtual void AddComponent(Visible2DComponent* component) = 0;
	virtual bool RemoveComponent(Visible2DComponent* component) = 0;
};
