#pragma once
#include <ZGI/Widgets/ZGIWidget.h>

class ZoranEngine_EXPORT ZGIProgressBar : public ZGIWidget
{
private:
	// This is what the progress bar gets filled with. It defaults to a simple spacer widget with a background color
	// To give ZGIProgressBar a custom animation / fill you must set this with SetProgressWidget. If you want there to be a background
	// Image to acompany the fill then set it with The ZGIBrush.
	ZGIWidget* progressWidget;
	// the current progress that represents how full the progress bar is in the range [0-1].
	// so 0.5 would make the bar half full
	float progress;
	bool needsUpdate;

private:
	void UpdateProgress();

public:
	ZGIProgressBar(ZGIVirtualWindow* owningWindow);
	~ZGIProgressBar();

	void SetProgressWidget(ZGIWidget* progress);
	void SetProgress(float progress); // range is [0.0 - 1.0]

	// Widget Overrides
	virtual void Render(const Matrix44& projection)override;
	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGIProgressBar"; }
};
