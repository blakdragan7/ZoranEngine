#pragma once
#include <ZGI\Panels\ZGIPanel.h>

struct GridSocket
{
	Vector2I gridPosition;
	Vector2I gridSize;

	AlignmentBit alignment;

	ZGIWidget* widget;

	GridSocket(Vec2I pos, Vec2I size, AlignmentBit alignment, ZGIWidget* widget) :
		gridPosition(pos), gridSize(size), alignment(alignment), widget(widget)
	{}

	GridSocket() : widget(0), alignment(0)
	{}

	bool operator==(ZGIWidget* widget)const
	{
		return this->widget == widget;
	}
	bool operator==(GridSocket& other)const
	{
		return widget == other.widget;
	}
	bool operator==(Vec2I position)
	{
		return (gridPosition <= position && (gridPosition + gridSize - 1) >= position);
	}
};

class ZoranEngine_EXPORT ZGIGridPanel : public ZGIPanel
{
private:
	std::vector<GridSocket> *grid;

	Vector2I gridSize; // grid size

	unsigned numWidgets;

private:
	void RepositionContents();
	std::vector<GridSocket>::iterator Find(ZGIWidget* widget)const;
	GridSocket* At(int x, int y);

public:
	ZGIGridPanel(int sizeX, int sizeY, ZGIVirtualWindow* owningWindow);
	ZGIGridPanel(Vec2I size, ZGIVirtualWindow* owningWindow);
	~ZGIGridPanel();

	void Resize(int sizeX, int sizeY);
	void Resize(Vec2I newSize);

	const GridSocket* At(int x, int y)const;

	bool AddWidget(ZGIWidget* widget, int x, int y, int rowSpan = 1, int columnSpawn = 1, AlignmentBit alignment = Alignment_Top | Alignment_Left);
	void RemoveWidget(ZGIWidget* widget);
	bool RemoveWidget(int x,int y);

	/* Panel Override */

	virtual void AnimateAllWidgets(float dt)override;

	virtual bool PositionIsEmpty(Vec2I position)const;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;
	virtual void Print(unsigned tabs)const override;
	virtual const char* GetClassString()const override { return "ZGIGridPanel"; }
};

