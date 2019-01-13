#include "stdafx.h"
#include "ZGIProgressBar.h"
#include <ZGI/Widgets/ZGISpacer.h>
#include <ZGI/Core/ZGIBrush.h>

void ZGIProgressBar::UpdateProgress()
{
	progressWidget->SetPosition(position);
	progressWidget->SetSize({ size.x * progress,size.y });
}

ZGIProgressBar::ZGIProgressBar(ZGIVirtualWindow* owningWindow) : progress(0), needsUpdate(0), ZGIWidget(owningWindow)
{
	progressWidget = new ZGISpacer(owningWindow);
	progressWidget->GetBrush()->SetBackgroudHue({0.0,1.0,0.0,1.0});
	progressWidget->SetDrawBrush(true);
	GetBrush()->SetBackgroudHue({ 0.5,0.5,0.5,1.0 });
	SetDrawBrush(true);
}

ZGIProgressBar::~ZGIProgressBar()
{
	delete progressWidget;
}

void ZGIProgressBar::SetProgressWidget(ZGIWidget * progress)
{
	if (progressWidget == progress)return;

	delete progressWidget;
	progressWidget = progress;
	needsUpdate = true;
}

void ZGIProgressBar::SetProgress(float progress)
{
	this->progress = progress;
	needsUpdate = true;
}

void ZGIProgressBar::Render(const Matrix44 & projection)
{
	if (isDirty || needsUpdate)
	{
		UpdateProgress();
		needsUpdate = false;
	}

	ZGIWidget::Render(projection);

	progressWidget->Render(projection);
}

void ZGIProgressBar::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher Stuff
}
