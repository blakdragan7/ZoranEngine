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
	/* bounds refers to the absolute size of the widget. I.E. an image widgets bounds is the size of the image 
	*  Bounds can only be set by the widget because only it should be able to determine what the absolute size would be */
	Vector2D bounds;
	/* size represents the size of the widget in widget space. */
	Vector2D size;
	/* position of the widget in widget space */
	Vector2D position;

	// render cache
	Matrix44 translate;
	Matrix44 rotation;
	Matrix44 modelCache;
	Matrix44 scale;

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

	// default does nothing simply to allow this method to not have to be overriden
	virtual void Animate(float dt);

	virtual void SetSize(Vec2D size);
	virtual void SetPosition(Vec2D position);

	virtual Vector2D GetBounds()const;

	// does not affect HitTest, so even if it is rotated, it will still receive mouse events as if it wasn't
	// therfore this is purely for rendering, not functionality
	virtual void SetRotation(float rotation);

	virtual Vector2D GetSize()const { return size; }
	inline Vec2D GetPosition()const { return position; }

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

	virtual bool RawKeyEvent(KeyEventType type, unsigned key)override { return false; };
	virtual bool CharEvent(unsigned uni)override { return false; }

	inline bool GetContainsMouse()const { return mouseHasEntered; }

	// Brush Variables

	inline void SetDrawBrush(bool draw) { shouldDrawBrush = draw; }
	inline ZGIBrush* GetBrush()const { return widgetBrush; }

	inline bool GetDrawBrush()const { return shouldDrawBrush; }

	// Info about widget

	virtual bool DoesContainText()const { return false; }
};

