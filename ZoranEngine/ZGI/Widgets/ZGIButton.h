#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

class GUIRenderer;
class ZoranEngine_EXPORT ZGIButton : public ZGIWidget
{
private:
	GUIRenderer * renderer;

public:
	ZGIButton(ZGIVirtualWindow* owningWindow);
	~ZGIButton();

	// widget override

	virtual void Render(const Matrix44& projection)override;
	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual bool MouseDown(const PlatformMouseBase* mouse)override;
	virtual bool MouseUp(const PlatformMouseBase* mouse)override;
	virtual bool MouseEnterd(const PlatformMouseBase* mouse)override;
	virtual bool MouseLeft(const PlatformMouseBase* mouse)override;
};
