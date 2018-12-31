#pragma once
#include <ZGI\Panels\ZGIPanel.h>
#include <ZGI/Core/ZGIBasicSocket.h>


class ZoranEngine_EXPORT ZGIOverlayPanel : public ZGIPanel
{
private:
	std::vector<ZGIBasicSocket>* sockets;
	bool needsSocketUpdate;

private:
	void UpdateSockets();

public:
	ZGIOverlayPanel(ZGIVirtualWindow* owningWindow);
	~ZGIOverlayPanel();
	
	void MoveWidgetForward(ZGIWidget* widget);
	void MoveWidgetBackward(ZGIWidget* widget);

	void AddWidget(ZGIWidget* widget);
	void RemoveWidget(ZGIWidget* widget);

	/* Panel Override */

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;
	virtual const char* GetClassString()const override { return "ZGIOverlayPanel"; }
};

