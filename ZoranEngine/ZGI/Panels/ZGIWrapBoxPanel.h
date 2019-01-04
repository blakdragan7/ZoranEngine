#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZoranEngine_EXPORT ZGIWrapBoxPanel : public ZGIPanel
{
private:
	std::vector<ZGIWidget*>* widgets;
	bool needsUpdate;

	Vector2D contentSize;

private:
	void UpdateWidgetPositions();

public:
	ZGIWrapBoxPanel(Vec2D contentSize, ZGIVirtualWindow* owningWindow);
	~ZGIWrapBoxPanel();
	
	void AddWidget(ZGIWidget* widget);
	void RemoveWidget(ZGIWidget* widget);

	inline void SetContentSize(Vec2D size) { contentSize = size; needsUpdate = true; }

	/* Panel Override */

	virtual void AnimateAllWidgets(float dt)override;

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

