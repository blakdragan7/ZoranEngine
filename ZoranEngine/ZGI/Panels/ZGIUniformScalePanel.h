#pragma once
#include <ZGI\Panels\ZGIPanel.h>
class ZoranEngine_EXPORT ZGIUniformScalePanel : public ZGIPanel
{
private:
	ZGIWidget*	content;
	Vector2D	contentStartingSize; // contents starting size

private:
	void RepositionContent();

public:
	ZGIUniformScalePanel(ZGIVirtualWindow* owningWindow);
	~ZGIUniformScalePanel();

	/* Pannel Override */

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;
	/* This assumes CanAddWidget was already called. Calling this in release mode without first checking CanAddWidget can result in a memory leak. */
	virtual void AddWidget(ZGIWidget* widget);
	virtual void RemoveWidget(ZGIWidget* widget);

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void SetSize(Vec2D size)override;
	virtual void SetPosition(Vec2D position)override;

	virtual void Render(const Matrix44& projection)override;
	virtual void Animate(float dt)override;
	virtual const char* GetClassString()const override { return "ZGIUniformScalePanel"; }
	virtual void Print(unsigned tabs)const override;

};

