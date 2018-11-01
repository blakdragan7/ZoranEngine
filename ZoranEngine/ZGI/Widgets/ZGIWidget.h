#pragma once
#include <ZGI\Core\ZGIBase.h>
#include <Interfaces/IMouseEventHandler.h>
#include <Interfaces/IKeyboardEventHandler.h>

class ZoranEngine_EXPORT ZGIWidget : public ZGIBase, public IMouseEventHandler, public IKeyboardEventHandler
{
private:
	bool mouseHasEntered;

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
	virtual ZGIWidget* HitTest(Vec2D pos);

	/*IMouseEventHandler Defaults*/

	virtual void MouseDown(const PlatformMouseBase*) {}
	virtual void MouseUp(const PlatformMouseBase*) {}
	virtual void MouseMove(const PlatformMouseBase*);

	virtual void MouseEnterd(const PlatformMouseBase*) {}
	virtual void MouseLeft(const PlatformMouseBase*) {}

	/*IKeyboardEvents Defaults*/

	virtual void KeyEvent(KeyEventType type, unsigned key) {};

	inline bool GetContainsMouse()const { return mouseHasEntered; }
};

