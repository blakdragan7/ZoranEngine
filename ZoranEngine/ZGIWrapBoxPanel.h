#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZGIWrapBoxPanel : public ZGIPanel
{
private:
	std::vector<ZGIWidget*>* widgets;
	bool needsUpdate;

private:
	void UpdateWidgetPositions();

public:
	ZGIWrapBoxPanel(ZGIVirtualWindow* owningWindow);
	~ZGIWrapBoxPanel();
	
	/* Panel Override */

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;

	virtual const char* GetClassString()const override { return "ZGIWrapBoxPanel"; }
};

