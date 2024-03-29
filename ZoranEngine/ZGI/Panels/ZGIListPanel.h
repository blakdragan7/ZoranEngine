#pragma once
#include <ZGI\Panels\ZGIPanel.h>

struct ListSocket
{
	ZGIWidget* widget;
	AlignmentBit alignment;

	bool operator ==(ZGIWidget* widget)
	{ return this->widget == widget; }
};

class ZoranEngine_EXPORT ZGIListPanel : public ZGIPanel
{
private:
	std::vector<ListSocket>* widgetList;
	unsigned maxListSize;
	bool dynamicSize;

private:
	void PositionAndSizeWidgets();

public:
	ZGIListPanel(bool dynamicSize, ZGIVirtualWindow* owningWindow);
	~ZGIListPanel();

	// used to allow custom re-ordering of the items in the list
	inline std::vector<ListSocket>* GetWidgetList() { return widgetList; }

	void AddWidget(ZGIWidget* widget, AlignmentBit alignment = Alignment_Center);
	// Used for spacing and sizing of widgets added to the list, there can actually be more 
	// widgets then this in the list but they will render past the bounds of the panel
	inline void SetListMaxSize(unsigned size) { maxListSize = size; }
	inline unsigned GetListMaxSize() { return maxListSize; }

	void Print(unsigned tabs)const override;
	virtual const char* GetClassString()const override { return "ZGIListPanel"; }

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
};

