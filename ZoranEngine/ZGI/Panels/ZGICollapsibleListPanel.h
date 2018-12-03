#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class TextureBase;
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

	TextureBase* collapsedImage;
	TextureBase* openImage;

	ZGIListPanel* list;

private:
	void RecalculateListSizeAndPosition();

public:
	ZGICollapsibleListPanel(ZGIVirtualWindow* owningWindow);
	~ZGICollapsibleListPanel();

	inline ZGIListPanel* GetList() { return list; }

	void SetCollapsedImage(std::string imagePath);
	void SetOpenImage(std::string imagePath);

	void SetCollapsedImage(TextureBase* image);
	void SetOpenImage(TextureBase* image);

	void SetCollapsed(bool collapsed);
	inline bool GetIsCollapsed()const { return isCollapsed; }

	virtual void Render(const Matrix44& projection)override;

	// ZGIListPanel passthroughs

	std::vector<ListSocket>* GetWidgetList();

	void AddWidget(ZGIWidget* widget, AlignmentBit alignment = Alignment_Center);
	void SetListMaxSize(unsigned size);
	unsigned GetListMaxSize();

	virtual bool KeyEventSub(KeyEventType type, unsigned key)override;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
};

