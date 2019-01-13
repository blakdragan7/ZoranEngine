#pragma once
#include <ZGI/Widgets/ZGIWidget.h>

#include <Rendering/Primitives.h>

class ZGIScrollPanel;
class ZGIVerticalBoxPanel;
class ZoranEngine_EXPORT ZGIComboBox : public ZGIWidget
{
private:
	ZGIVerticalBoxPanel* vBox;
	ZGIScrollPanel* panel;

	ZGIWidget* highLighter;
	ZGIWidget* selectedItem;
	ZGIWidget* currentItem;

	bool isExpanded;
	bool itemsNeedUpdate;

	Vector2D expandedSize;
	Vector2D collapsedSize;
	Vector2D collapsedPosition;

	Color hoverHue;

	int animateDirection;
	float animationTime;

protected:
	virtual void ItemSelected(ZGIWidget* item);
	void UpdateItems();

public:
	ZGIComboBox(ZGIVirtualWindow* owningWindow);
	~ZGIComboBox();

	void AddItem(ZGIWidget* item);
	void RemoveItem(ZGIWidget* item);

	// Widget Overrides

	virtual void Render(const Matrix44& projection)override;

	void Animate(float dt)override;

	virtual void SetSize(Vec2D size)override;
	virtual void SetPosition(Vec2D position)override;

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGIComboBox"; }

	// MouseEvents

	virtual bool MouseUp(const PlatformMouseBase&)override;
	virtual bool MouseDown(const PlatformMouseBase&)override;
	virtual bool MouseMove(const PlatformMouseBase&)override;
	virtual bool MouseEnterd(const PlatformMouseBase&)override;
	virtual bool MouseLeft(const PlatformMouseBase&)override;
	virtual bool MouseScroll(const PlatformMouseBase&, float scrollAmount)override;
};
