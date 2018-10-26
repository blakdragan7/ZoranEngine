#pragma once
#include <ZGI\Panels\ZGIPanel.h>

class ZoranEngine_EXPORT ZGIUniformScalePanel : public ZGIPanel
{
private:
	ZGIWidget*	content;

	Vector2D	contentStartingSize; // contents starting size

public:
	ZGIUniformScalePanel();
	~ZGIUniformScalePanel();

	/* Pannel Override */
	virtual bool CanAddWidget(ZGIWidget* widget)const override;
	/* This assumes CanAddWidget was already called. Calling this in release mode without first checking CanAddWidget can result in a memory leak. */
	virtual void AddWidget(ZGIWidget* widget)override;
	virtual void RemoveWidget(ZGIWidget* widget)override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void SetSize(Vec2D size)override;
	virtual void SetPosition(Vec2D position)override;

	virtual void Render(const Matrix44& projection)override;

};

