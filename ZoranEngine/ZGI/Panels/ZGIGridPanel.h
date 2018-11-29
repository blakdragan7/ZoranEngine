#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZoranEngine_EXPORT ZGIGridPanel : public ZGIPanel
{
private:
	std::vector<ZGIWidget*> *grid;

	Vector2I size; // sizeCache
	Vector2I nextPosition; // used in addWidget

	unsigned numWidgets;

private:
	void RepositionContents();

public:
	ZGIGridPanel(int sizeX, int sizeY, ZGIVirtualWindow* owningWindow);
	ZGIGridPanel(Vec2I size, ZGIVirtualWindow* owningWindow);
	~ZGIGridPanel();

	void Resize(int sizeX, int sizeY);
	void Resize(Vec2I newSize);

	inline Vector2I positionFrom(int index) { return Vector2I(index % size.w, index / size.w); }
	inline int IndexOf(Vec2I position)const { return (position.y * size.w) + position.x; }
	inline int IndexOf(int x, int y)const { return (y * size.w) + x; }

	ZGIWidget* At(int x, int y)const;

	// to remove widget at location call this with 0 for widget
	bool AddWidget(ZGIWidget* widget, int x, int y);

	/* Panel Override */

	virtual bool KeyEventSub(KeyEventType type, unsigned key)override;

	virtual bool PositionIsEmpty(Vec2I position)const;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;
};

