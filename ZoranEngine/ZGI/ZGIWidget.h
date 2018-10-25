#pragma once
#include <ZGI\ZGIBase.h>

class ZoranEngine_EXPORT ZGIWidget : public ZGIBase
{
protected:
	Vector2D position;
	Vector2D size;
	Matrix44 scale;
	Matrix44 translate;
	Matrix44 rotation;
	Matrix44 modelCache;

protected:
	void RecalculateModelCache();

public:
	ZGIWidget();
	~ZGIWidget();

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize) = 0;
	virtual void Render(const Matrix44& projection) = 0;

	virtual void SetSize(Vec2D size);
	virtual void SetPosition(Vec2D position);

	virtual void SetRotation(float rotation);

	inline Vec2D GetSize() { return size; }
	inline Vec2D GetPosition() { return position; }

};

