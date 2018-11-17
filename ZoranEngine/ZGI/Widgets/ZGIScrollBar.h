#pragma once
#include<ZGI\Widgets\ZGIWidget.h>

class GUIColoredRectRenderer;
class ZGIScrollBar : public ZGIWidget
{
private:
	GUIColoredRectRenderer* renderer;
	// the normalzied size of the moving scroll bar, the widget size is the size of the
	// entire scroll bar so essentially the extents
	float sizeAlpha; 
	// 0 means far left or top 1 means far right or bottom
	float scrollAlpha;
	// cache of the MVP
	Matrix44 scrollMVP;
	// determines orientation and effects how size alpha and scroll alpha are interpreted
	bool isVertical;

private:
	void SizeAndPositionScrollBar();

public:
	ZGIScrollBar(bool isVertical, ZGIVirtualWindow* owningWindow);
	~ZGIScrollBar();

	inline void SetScrollPositionAlpha(float alpha) { scrollAlpha = alpha; isDirty = true; }
	inline float GetScrollPositionAlpha()const { return scrollAlpha; }
	inline void SetScrollBarSizeAlpha(float size) { sizeAlpha = size;  isDirty = true; }
	inline float GetScrollBarSizeAlpha()const { return sizeAlpha; }

	// widget override

	virtual void Render(const Matrix44& mvp)override;
	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
};

