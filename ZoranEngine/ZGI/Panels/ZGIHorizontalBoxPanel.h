#pragma once
#include <ZGI\Panels\ZGIPanel.h>
#include <ZGI/Core/ZGIBasicSocket.h>

class ZoranEngine_EXPORT ZGIHorizontalBoxPanel : public ZGIPanel
{
private:
	std::vector<ZGIBasicSocket>* sockets;
	bool needsSocketsSized;

private:
	void UpdateSocketPositions();


public:
	ZGIHorizontalBoxPanel(ZGIVirtualWindow* owningWindow);
	~ZGIHorizontalBoxPanel();
	
	void MoveWidgetRight(ZGIWidget* widget);
	void MoveWidgetLeft(ZGIWidget* widget);

	void AddWidget(ZGIWidget* widget, float size = 1.0f, int index = -1);
	void RemoveWidget(ZGIWidget* widget);
	ZGIWidget* RemoveWidget(int index);
	void SetSizeForWidget(float size, ZGIWidget* widget);
	void SetSizeForIndex(float size, int index);
	void CommitSizes();

	const ZGIBasicSocket* SocketForIndex(int index)const;

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

	virtual const char* GetClassString()const override { return "ZGIHorizontalBoxPanel"; }
};

