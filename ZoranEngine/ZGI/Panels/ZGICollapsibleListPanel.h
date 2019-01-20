#pragma once
#include <ZGI\Panels\ZGIPanel.h>

#include <Resources/ImageResource.h>

class ZGIImage;
class ZGIButton;
class ZGILabel;
class ZGIListPanel;
class ZGIGridPanel;
struct ListSocket;
class ZoranEngine_EXPORT ZGICollapsibleListPanel : public ZGIPanel
{
private:
	bool isCollapsed;

	ZGIGridPanel* header;
	ZGIButton* headerButton;
	ZGILabel* headerLabel;

	ImageResource collapsedImage;
	ImageResource openImage;

	ZGIListPanel* list;

private:
	void RecalculateListSizeAndPosition();

public:
	ZGICollapsibleListPanel(bool dynamicSize, ZGIVirtualWindow* owningWindow);
	~ZGICollapsibleListPanel();

	inline ZGIListPanel* GetList() { return list; }

	void SetHeaderText(std::string text);

	void SetCollapsedImage(std::string imagePath);
	void SetOpenImage(std::string imagePath);

	void SetCollapsedImage(ImageResource image);
	void SetOpenImage(ImageResource image);

	void SetCollapsed(bool collapsed);
	inline bool GetIsCollapsed()const { return isCollapsed; }

	virtual void Render(const Matrix44& projection)override;

	// ZGIListPanel passthroughs

	virtual void AnimateAllWidgets(float dt)override;

	std::vector<ListSocket>* GetWidgetList();

	void AddWidget(ZGIWidget* widget, AlignmentBit alignment = Alignment_Center);
	void SetListMaxSize(unsigned size);
	unsigned GetListMaxSize();

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGICollapsibleListPanel"; }
	virtual void Print(unsigned tabs)const override;
};

