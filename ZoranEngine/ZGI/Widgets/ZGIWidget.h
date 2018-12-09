#pragma once
#include <ZGI\Core\ZGIBase.h>
#include <Interfaces/IMouseEventHandler.h>
#include <Interfaces/IKeyboardEventHandler.h>

class ZGIVirtualWindow;
class ZGIBrush;
class ZoranEngine_EXPORT ZGIWidget : public ZGIBase, public IMouseEventHandler, public IKeyboardEventHandler
{
private:
	bool mouseHasEntered;

protected:
	Vector2D bounds;
	Vector2D position;
	Vector2D size;
	Matrix44 scale;

	// render cache
	Matrix44 translate;
	Matrix44 rotation;
	Matrix44 modelCache;

	// owming window
	ZGIVirtualWindow* owningWindow;

	bool isDirty; // used to update rendering
	ZGIBrush* widgetBrush;
	bool shouldDrawBrush;

protected:
	void RecalculateModelCache();

public:
	ZGIWidget(ZGIVirtualWindow* owningWindow);
	~ZGIWidget();
	virtual const char* GetClassString()const = 0;
	virtual void Print(unsigned tabs)const;
	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize) = 0;

	// default draws debug info and update Debug render when dirty
	virtual void Render(const Matrix44& projection);

	virtual void SetSize(Vec2D size);
	virtual void SetPosition(Vec2D position);

	virtual void SetBounds(Vec2D bounds);
	virtual Vector2D GetBounds() { return bounds; }

	// does not affect HitTest, so even if it is rotated, it will still receive mouse events as if it wasn't
	// therfore this is purely for rendering, not functionality
	virtual void SetRotation(float rotation);

	virtual Vector2D GetSize() { return size; }
	inline Vec2D GetPosition() { return position; }

	// standard 2d hit test
	// note the default does not take into account rotation
	// This also assumes that pos is in the owningWindow's virtual space and not absolute space
	virtual ZGIWidget* HitTest(Vec2D pos);

	/*IMouseEventHandler Defaults*/

	virtual bool MouseDown(const PlatformMouseBase*)override { return false; }
	virtual bool MouseUp(const PlatformMouseBase*)override { return false; }
	virtual bool MouseMove(const PlatformMouseBase*);
			
	virtual bool MouseEnterd(const PlatformMouseBase*)override { return false; }
	virtual bool MouseLeft(const PlatformMouseBase*)override { return false; }

	/*IKeyboardEvents Defaults*/

	virtual bool KeyEvent(KeyEventType type, unsigned key)override { return false; };

	inline bool GetContainsMouse()const { return mouseHasEntered; }

	// Brush Variables

	inline void SetDrawBrush(bool draw) { shouldDrawBrush = draw; }
	inline ZGIBrush* GetBrush() { return widgetBrush; }

	inline bool GetDrawBrush()const { return shouldDrawBrush; }

	// Info about widget

	virtual bool DoesContainText()const { return false; }
};

