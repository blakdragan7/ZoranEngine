#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZGITabHeader;
class ZGITabHeaderPanel : public ZGIPanel
{
private:
	std::vector<ZGITabHeader*>* headers;
	int activeHeader;

private:
	void AddHeader(ZGITabHeader* header);
	void AddHeader(ZGITabHeader* header, int index);

public:
	ZGITabHeaderPanel(ZGIVirtualWindow* owningWindow);
	~ZGITabHeaderPanel();
	
	void AddHeader(const std::string& header);
	void InsertHeader(int index, const std::string& header);

	void SetHeaderActive(int index);
	int GetActiveIndex()const;

	void RemoveHeaderForIndex(int index);
	ZGITabHeader* RemoveHeaderNoDelete(int index);
	void RemoveHeaderNoDelete(ZGITabHeader* header);

	int IndexForName(const std::string& name)const;

	/* Panel Override */

	virtual void AnimateAllWidgets(float dt)override;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual void RemoveWidget(ZGIWidget* widget)override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;

	virtual const char* GetClassString()const override { return "ZGITabHeaderPanel"; }

};


