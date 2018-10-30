#pragma once
#include <ZGI\Core\ZGIBase.h>
#include <Interfaces/IMouseEventHandler.h>
#include <Interfaces/IKeyboardEvents.h>

class MouseInstance;
class ZoranEngine_EXPORT ZGIWidget : public ZGIBase, public IMouseEventHandler, public IKeyboardEvents
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

	// does not affect HitTest, so even if it is rotated, it will still receive mouse events as if it wasn't
	// therfore this is purely for rendering, not functionality
	virtual void SetRotation(float rotation);

	inline Vec2D GetSize() { return size; }
	inline Vec2D GetPosition() { return position; }

	// standard 2d hit test
	// note the default does not take into account rotation
	virtual bool HitTest(Vec2D pos);

	/*IMouseEventHandler Defaults*/

	virtual void MouseDown(const MouseInstance&) {};
	virtual void MouseMove(const MouseInstance&) {};
	virtual void MouseUp(const MouseInstance&) {};

	virtual void MouseEnterd(const MouseInstance&) {}
	virtual void MouseLeft(const MouseInstance&) {}

	/*IKeyboardEvents Defaults*/

	virtual void KeyEvent(KeyEventType type, unsigned key) {};
};

