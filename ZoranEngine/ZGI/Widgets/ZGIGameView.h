#pragma once
#include <ZGI/Widgets/ZGIWidget.h>

class PlayerInstanceBase;
class ZGIVirtualViewport;
class ZoranEngine_EXPORT ZGIGameView : public ZGIWidget
{
private:
	PlayerInstanceBase* player;
	ZGIVirtualViewport* viewport;

public:
	ZGIGameView(ZGIVirtualWindow* owningWindow);
	~ZGIGameView();

	void SetPlayer(PlayerInstanceBase* instance);

	// widget overrides

	virtual void Render(const Matrix44& projection)override;

	ZGIWidget* HitTest(Vec2D pos)override;

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGIGameView"; }
};

