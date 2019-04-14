#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZGITabHeaderPanel;
class ZGISwitcherPanel;
class ZGIVerticalBoxPanel;
class ZoranEngine_EXPORT ZGITabPanel : public ZGIPanel
{
private:
	ZGISwitcherPanel* body;
	ZGITabHeaderPanel* header;
	ZGIVerticalBoxPanel* panel;

public:
	ZGITabPanel(ZGIVirtualWindow* owningWindow);
	~ZGITabPanel();
	
	void AddTab(const std::string& tabName, ZGIWidget* widget);
	void RemoveTab(const std::string& tabName);
	void RemoveTab(int index);

	void SetIndexActive(int index);

	/* Panel Override */

	void RemoveWidget(ZGIWidget *)override;

	void AnimateAllWidgets(float dt)override;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;

	virtual const char* GetClassString()const override { return "ZGITabPanel"; }
};

