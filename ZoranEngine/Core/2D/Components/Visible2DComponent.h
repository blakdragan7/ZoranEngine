#pragma once
#include <Core\Components\VisibleComponentBase.h>
#include <Core/2D/Components/Component2DBase.h>

class ZoranEngine_EXPORT Visible2DComponent : public VisibleComponentBase, public Component2DBase
{
private:
	bool shouldRender;

public:
	Visible2DComponent(unsigned renderLayer);
	~Visible2DComponent();

	virtual void PreRender()override;
	virtual void Render()override;
	virtual void PostRender()override;

	inline void SetIsVisible(bool shouldRender) { this->shouldRender = shouldRender; }
	inline bool GetIsVisible() { return shouldRender; }
};

