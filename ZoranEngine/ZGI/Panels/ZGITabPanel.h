#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZGISwitcherPanel;
class ZoranEngine_EXPORT ZGITabPanel : public ZGIPanel
{
private:
	ZGISwitcherPanel* body;

public:
	ZGITabPanel(ZGIVirtualWindow* owningWindow);
	~ZGITabPanel();
	
	void AddWidget(ZGIWidget* widget);
	void RemoveWidget(ZGIWidget* widget);

	/* Panel Override */

	void AnimateAllWidgets(float dt)override;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;
};

