#pragma once
#include <ZGI\Core\ZGIBase.h>
#include <Interfaces/IMouseEventHandler.h>
#include <Interfaces/IKeyboardEventHandler.h>

class ZGIVirtualWindow;
class LineLoopRenderer;
class ZoranEngine_EXPORT ZGIWidget : public ZGIBase, public IMouseEventHandler, public IKeyboardEventHandler
{
private:
	bool mouseHasEntered;
	LineLoopRenderer* renderer;

protected:
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
	bool drawDebugView;
	bool drawEditorView;

protected:
	void RecalculateModelCache();
	void UpdateDebugRender();

public:
	ZGIWidget(ZGIVirtualWindow* owningWindow);
	~ZGIWidget();

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize) = 0;

	// default draws debug info and update Debug render when dirty
	virtual void Render(const Matrix44& projection);

	virtual void SetSize(Vec2D size);
	virtual void SetPosition(Vec2D position);

	// does not affect HitTest, so even if it is rotated, it will still receive mouse events as if it wasn't
	// therfore this is purely for rendering, not functionality
	virtual void SetRotation(float rotation);

	inline Vec2D GetSize() { return size; }
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

	// Debug / Editor Variables

	inline void SetDrawDebugView(bool d) { drawDebugView = d; }
	inline void SetDrawEditorView(bool d) { drawEditorView = d; }

	inline bool GetDrawDebugView()const { return drawDebugView; }
	inline bool GetDrawEditorView()const { return drawEditorView; }
};

