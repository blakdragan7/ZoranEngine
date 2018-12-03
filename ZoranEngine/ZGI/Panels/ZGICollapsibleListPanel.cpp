#include "stdafx.h"
#include "ZGICollapsibleListPanel.h"
#include <ZGI/Core/ZGIBrush.h>
#include <ZGI/Panels/ZGIListPanel.h>
#include <ZGI/Panels/ZGIGridPanel.h>
#include <ZGI/Widgets/ZGIButton.h>
#include <ZGI/Widgets/ZGIImage.h>
#include <ZGI/Widgets/ZGILabel.h>

#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureManager.h>

void ZGICollapsibleListPanel::RecalculateListSizeAndPosition()
{
	header->SetSize(Vector2D(size.w, size.h * 0.1f));
	header->SetPosition(position + Vector2D(0, size.h * 0.9f));

	list->SetSize(size * 0.9f);
	list->SetPosition(position + Vector2D(size.w * 0.1f, 0));
}

ZGICollapsibleListPanel::ZGICollapsibleListPanel(ZGIVirtualWindow* owningWindow) : ZGIPanel(owningWindow)
{
	list = new ZGIListPanel(owningWindow);
	header = new ZGIGridPanel(10,1,owningWindow);
	headerLabel = new ZGILabel(owningWindow);
	headerButton = new ZGIButton(owningWindow);

	header->AddWidget(headerButton, 0, 0);
	header->AddWidget(headerLabel, 1, 0, 9);

	headerButton->SetButtonReleasedFunction([this]() {SetCollapsed(!this->GetIsCollapsed()); });

	headerLabel->SetAlignment(Alignment_Center);
	headerLabel->SetFontSize(30);
	headerLabel->SetText("This is a test list");

	rEngine->EnableAlpha();

	collapsedImage = tManager->TextureForFilePath("right-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	openImage = tManager->TextureForFilePath("down-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);

	SetCollapsed(false);
}

ZGICollapsibleListPanel::~ZGICollapsibleListPanel()
{
	delete header;
	delete list;

	tManager->DestroyTexture(collapsedImage);
	tManager->DestroyTexture(openImage);
}

void ZGICollapsibleListPanel::SetCollapsedImage(std::string imagePath)
{
	tManager->DestroyTexture(collapsedImage);
	collapsedImage = tManager->TextureForFilePath(imagePath.c_str());
}

void ZGICollapsibleListPanel::SetOpenImage(std::string imagePath)
{
	tManager->DestroyTexture(openImage);
	openImage = tManager->TextureForFilePath(imagePath.c_str());
}

void ZGICollapsibleListPanel::SetCollapsedImage(TextureBase * image)
{
	tManager->DestroyTexture(collapsedImage);
	collapsedImage = image;
}

void ZGICollapsibleListPanel::SetOpenImage(TextureBase * image)
{
	tManager->DestroyTexture(openImage);
	openImage = image;
}

void ZGICollapsibleListPanel::SetCollapsed(bool collapsed)
{
	isCollapsed = collapsed;

	if (isCollapsed)
	{
		headerButton->GetBrush()->SetBackgroudImage(collapsedImage);
	}
	else
	{
		headerButton->GetBrush()->SetBackgroudImage(openImage);
	}
}

void ZGICollapsibleListPanel::Render(const Matrix44 & projection)
{
	if (isDirty)
		RecalculateListSizeAndPosition();

	header->Render(projection);

	if (isCollapsed == false)
		list->Render(projection);
	else
		ZGIPanel::Render(projection);
}

std::vector<ListSocket>* ZGICollapsibleListPanel::GetWidgetList()
{
	return list->GetWidgetList();
}

void ZGICollapsibleListPanel::AddWidget(ZGIWidget * widget, AlignmentBit alignment)
{
	list->AddWidget(widget,alignment);
}

void ZGICollapsibleListPanel::SetListMaxSize(unsigned size)
{
	list->SetListMaxSize(size);
}

unsigned ZGICollapsibleListPanel::GetListMaxSize()
{
	return list->GetListMaxSize();
}

bool ZGICollapsibleListPanel::KeyEventSub(KeyEventType type, unsigned key)
{
	return list->KeyEventSub(type,key);
}

bool ZGICollapsibleListPanel::ContainsWidget(ZGIWidget * widget) const
{
	return list->ContainsWidget(widget);
}

bool ZGICollapsibleListPanel::CanAddWidget(ZGIWidget * widget) const
{
	return list->CanAddWidget(widget);
}

int ZGICollapsibleListPanel::GetNumberOfWidgets() const
{
	return list->GetNumberOfWidgets();
}

int ZGICollapsibleListPanel::GetMaxNumberOfWidgets() const
{
	return list->GetMaxNumberOfWidgets();
}

ZGIWidget * ZGICollapsibleListPanel::WidgetForPosition(Vec2D pos)
{
	if (ZGIWidget* w = headerButton->HitTest(pos))return w;
	if (ZGIWidget* w = headerLabel->HitTest(pos))return w;
	return list->WidgetForPosition(pos);
}

void ZGICollapsibleListPanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher Stuff
	list->ContainerResized(newSize,oldSize);
}
