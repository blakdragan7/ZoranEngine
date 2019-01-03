#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZoranEngine_EXPORT ZGISwitcherPanel : public ZGIPanel
{
private:
	std::vector<ZGIWidget*>* widgets;
	int activeWidget;

public:
	ZGISwitcherPanel(ZGIVirtualWindow* owningWindow);
	~ZGISwitcherPanel();
	
	void AddWidget(ZGIWidget* widget);
	void RemoveWidget(ZGIWidget* widget);

	void SetWidgetActive(ZGIWidget* widget);
	void SetIndexActive(int index);

	/* Panel Override */

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;

	virtual const char* GetClassString()const override { return "ZGISwitcherPanel"; }
};

