#include "stdafx.h"
#include "ZGIComboBox.h"

#include <ZGI/Windows/ZGIVirtualWindow.h>

#include <ZGI/Widgets/ZGISpacer.h>

#include <ZGI/Panels/ZGIScrollPanel.h>
#include <ZGI/Panels/ZGIVerticalBoxPanel.h>

#include <ZGI/Core/ZGIBrush.h>

#include <Core/PlatformMouseBase.h>

#include <Math/MathLib.h>

void ZGIComboBox::ItemSelected(ZGIWidget* item)
{
	selectedItem = item;
}

void ZGIComboBox::UpdateItems()
{

	expandedSize = { collapsedSize.x, 4.0f * collapsedSize.y };
	Vector2D vBoxSize(collapsedSize.x, (float)vBox->GetNumberOfWidgets() * collapsedSize.y);

	Vector2D position_n = collapsedPosition;
	position_n.y -= (((vBoxSize - collapsedSize)).h + collapsedSize.h);

	vBox->SetSize(vBoxSize);
	vBox->SetPosition(position_n);

	if (selectedItem)
	{
		selectedItem->SetSize(collapsedSize);
		selectedItem->SetPosition(collapsedPosition);
	}
}

ZGIComboBox::ZGIComboBox(ZGIVirtualWindow* owningWindow) : itemsNeedUpdate(false), animationTime(0), currentItem(0), animateDirection(0), selectedItem(0), isExpanded(false), ZGIWidget(owningWindow)
{
	vBox = new ZGIVerticalBoxPanel(owningWindow);
	panel = new ZGIScrollPanel(owningWindow);

	highLighter = new ZGISpacer(owningWindow);

	highLighter->SetDrawBrush(true);
	highLighter->GetBrush()->SetBackgroudHue({0.6f,0.6f,0.6f,0.3f});

	panel->AddWidget(vBox);

	panel->SetDrawBrush(true);
	panel->GetBrush()->SetBackgroudHue({0.3f,0.3f,0.3f,1.0f});

	hoverHue.r = 0.6f;
	hoverHue.g = 0.6f;
	hoverHue.b = 0.6f;
	hoverHue.a = 0.3f;

	SetDrawBrush(true);
}

ZGIComboBox::~ZGIComboBox()
{
	delete vBox;
	delete panel;
	delete highLighter;
}

void ZGIComboBox::AddItem(ZGIWidget * item)
{
	vBox->AddWidget(item);
	if (selectedItem == 0)
		selectedItem = item;

	itemsNeedUpdate = true;
}

void ZGIComboBox::RemoveItem(ZGIWidget * item)
{
	vBox->RemoveWidget(item);
}

void ZGIComboBox::Render(const Matrix44 & projection)
{
	if (isDirty || itemsNeedUpdate)
	{
		UpdateItems();
		itemsNeedUpdate = false;
	}

	ZGIWidget::Render(projection);

	if (isExpanded || animateDirection != 0)
	{
		if (selectedItem)selectedItem->RenderWithPositionAndSize(collapsedPosition,collapsedSize,projection);
		panel->Render(projection);
		if (currentItem)
		{
			highLighter->Render(projection);
		}
	}
	else
	{
		if (selectedItem)selectedItem->Render(projection);
	}
}

void ZGIComboBox::Animate(float dt)
{
	if (isExpanded)
	{
		panel->Animate(dt);
	}

	if (animateDirection != 0)
	{
		animationTime += dt * 2.0f;

		switch (animateDirection)
		{
		case 1:
		{
			Vector2D newSize = MathLib::lerp(collapsedSize, expandedSize, animationTime * animationTime);
			Vector2D newPosition = collapsedPosition;

			newPosition.y -= (((newSize - collapsedSize)).h + collapsedSize.h);

			panel->SetSize(newSize);
			panel->SetPosition(newPosition);

			size = newSize;
			position = newPosition;

			isDirty = true;
		}
		break;
		case -1:
		{
			Vector2D newSize = MathLib::lerp(expandedSize, collapsedSize, animationTime * animationTime);
			Vector2D newPosition = collapsedPosition;

			newPosition.y -= (((newSize - collapsedSize)).h + collapsedSize.h);

			panel->SetSize(newSize);
			panel->SetPosition(newPosition);

			size = newSize;
			position = newPosition;

			isDirty = true;
		}
		break;
		case 0:
		default:
			break;
		}

		if (animationTime >= 1.0f)
		{
			animationTime = 0;
			animateDirection = 0;
			if (isExpanded == false)
			{
				SetPosition(collapsedPosition);
				currentItem = 0;
			}
		}
	}
}

void ZGIComboBox::SetSize(Vec2D size)
{
	collapsedSize = size;
	ZGIWidget::SetSize(size);
}

void ZGIComboBox::SetPosition(Vec2D position)
{
	collapsedPosition = position;
	ZGIWidget::SetPosition(position);
}

void ZGIComboBox::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher Stuff
}

bool ZGIComboBox::MouseUp(const PlatformMouseBase &m)
{
	if (isExpanded)
	{
		isExpanded = false;

		auto selectedItemNow = panel->WidgetForPosition(owningWindow->ConvertAbsoluteToVirtual(m.GetPosition()));
		if (selectedItemNow)
		{
			selectedItem = selectedItemNow;
		}
		animateDirection = -1;

		ItemSelected(selectedItem);

		return panel->MouseUp(m);
	}
	else
	{
		isExpanded = true;
		animateDirection = 1;
		widgetBrush->SetBackgroudHue(Color::Clear);

	}

	return true;
}

bool ZGIComboBox::MouseDown(const PlatformMouseBase &m)
{
	if (isExpanded)return panel->MouseDown(m);
	return false;
}

bool ZGIComboBox::MouseMove(const PlatformMouseBase &m)
{
	if (isExpanded)
	{
		Vector2D translatedPos = owningWindow->ConvertAbsoluteToVirtual(m.GetPosition());

		if (auto w = panel->WidgetForPosition(translatedPos))
		{
			if (currentItem != w)
			{
				currentItem = w;
				highLighter->SetPosition(currentItem->GetPosition() + panel->GetScrollOffset());
				highLighter->SetSize(currentItem->GetSize());
			}
		}

		return panel->MouseMove(m);
	}
	return ZGIWidget::MouseMove(m);
}

bool ZGIComboBox::MouseEnterd(const PlatformMouseBase &m)
{
	if(isExpanded == false)widgetBrush->SetBackgroudHue(hoverHue);
	
	return false;
}

bool ZGIComboBox::MouseLeft(const PlatformMouseBase &m)
{
	widgetBrush->SetBackgroudHue(Color::Clear);

	return false;
}

bool ZGIComboBox::MouseScroll(const PlatformMouseBase &m, float scrollAmount)
{
	if (isExpanded)
	{
		 bool r = panel->MouseScroll(m, scrollAmount);
		 Vector2D translatedPos = owningWindow->ConvertAbsoluteToVirtual(m.GetPosition());

		 if (auto w = panel->WidgetForPosition(translatedPos))
		 {
			 if (currentItem != w)
			 {
				 currentItem = w;
				 highLighter->SetPosition(currentItem->GetPosition() + panel->GetScrollOffset());
				 highLighter->SetSize(currentItem->GetSize());
			 }
		 }
		 return r;
	}
	return false;
}
