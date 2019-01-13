#pragma once
#include <Core\Components\VisibleComponentBase.h>
#include <Core/2D/Components/Component2DBase.h>

class RenderEngine2DBase;
class ZoranEngine_EXPORT Visible2DComponent : public VisibleComponentBase, public Component2DBase
{
private:
	bool shouldRender;
	unsigned renderLayer;

protected:
	RenderEngine2DBase * engine2D;

public:
	Visible2DComponent(unsigned renderLayer,RenderEngine2DBase* engine=0);
	~Visible2DComponent();

	virtual void PreRender()override;
	virtual void Render(const Matrix44& cameraMatrix)override;
	virtual void PostRender()override;

	inline void SetIsVisible(bool shouldRender) { this->shouldRender = shouldRender; }
	inline bool GetIsVisible() { return shouldRender; }

	virtual void SetShaderProgram(ShaderProgramBase* program)override;

	void  SetRenderLayer(unsigned layer);
	inline unsigned GetRenderLayer() { return renderLayer; }
};

