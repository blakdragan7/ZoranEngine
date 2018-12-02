#pragma once
#include <ZGI\Panels\ZGIPanel.h>
class ZGIListPanel;
struct TreeSocket
{
	ZGIListPanel* list;
};

class ZoranEngine_EXPORT ZGITreePanel : public ZGIPanel
{
private:
	TreeSocket * baseSocket;

public:
	ZGITreePanel(ZGIVirtualWindow* owningWindow);
	~ZGITreePanel();

	/* Panel Override */

	virtual bool KeyEventSub(KeyEventType type, unsigned key)override;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;
};

