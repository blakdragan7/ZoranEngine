#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

class ZoranEngine_EXPORT ZGIPanel : public ZGIWidget
{
protected:
	bool drawDebugView;
	bool drawEditorView;

public:
	ZGIPanel();
	~ZGIPanel();

	virtual bool CanAddWidget(ZGIWidget* widget)const = 0;
	virtual void AddWidget(ZGIWidget* widget) = 0;
	virtual void RemoveWidget(ZGIWidget* widget) = 0;

	virtual int GetNumberOfWidgets()const = 0;
	virtual int GetMaxNumberOfWidgets()const = 0;

	// Generally, calls hittest on every widget with pos and returns the first to return true or 0 if none do
	virtual ZGIWidget* WidgetForPosition(Vec2D pos) = 0;

	// widget override
	virtual ZGIWidget* HitTest(Vec2D pos)override;
	virtual void MouseMove(const PlatformMouseBase* m)override;

	virtual void MouseDown(const PlatformMouseBase*)override;
	virtual void MouseUp(const PlatformMouseBase*)override;

	// Debug / Editor Variables

	inline void SetDrawDebugView(bool d) { drawDebugView = d; }
	inline void SetDrawEditorView(bool d) { drawEditorView = d; }

	inline bool GetDrawDebugView()const  { return drawDebugView; }
	inline bool GetDrawEditorView()const { return drawEditorView; }
};
