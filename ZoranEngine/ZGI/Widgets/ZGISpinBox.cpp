#include "stdafx.h"
#include "ZGISpinBox.h"

#include <string>
#include <sstream>
#include <iomanip>

#include <Utils/HighPrecisionClock.h>

#include <ZGI/Core/ZGIBrush.h>

#include <ZGI/Widgets/ZGIButton.h>
#include <ZGI/Widgets/ZGILabel.h>

#include <ZGI/Panels/ZGIGridPanel.h>

#include <Interfaces/IZGISpinBoxEventHandler.h>

void ZGISpinBox::ValueChanged(ZGISpinBox * b, float val)
{
	if(isWholeNumber)
		valueLabel->SetText(std::to_string((int)val));
	else
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << val;
		valueLabel->SetText(ss.str());
	}

	if (handler)handler->ValueChanged(b, val);
}

ZGISpinBox::ZGISpinBox(bool isWholeNumber, float val, float shiftVal, ZGIVirtualWindow * owningWindow) : isWholeNumber(isWholeNumber), 
handler(0), val(val), shiftVal(shiftVal), spinDirection(0), ZGIWidget(owningWindow)
{
	clock = new HighPrecisionClock;

	SetDrawBrush(true);
	widgetBrush->SetBackgroudHue({ 0.4f,0.4f,0.4f,1.0f });
	
	upButton = new ZGIButton(owningWindow);
	downButton = new ZGIButton(owningWindow);
	valueLabel = new ZGILabel(owningWindow);

	upButton->SetEventHandler(this);
	downButton->SetEventHandler(this);

	upButton->GetBrush()->SetBackgroudImage("up-arrow.png");
	downButton->GetBrush()->SetBackgroudImage("down-arrow.png");

	upButton->SetDrawBrush(true);
	downButton->SetDrawBrush(true);

	panel = new ZGIGridPanel(3,2,owningWindow);

	panel->AddWidget(valueLabel, 0, 0, 2, 2);
	panel->AddWidget(upButton, 2, 1);
	panel->AddWidget(downButton, 2,0);

	if (isWholeNumber)
		valueLabel->SetText(std::to_string((int)val));
	else
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << val;
		valueLabel->SetText(ss.str());
	}
}

ZGISpinBox::~ZGISpinBox()
{
	delete upButton;
	delete downButton;
	delete valueLabel;

	delete clock;

	delete panel;
}

void ZGISpinBox::SetIsWholeNumber(bool isWholeNumber_)
{
	isWholeNumber = isWholeNumber_;

	if (isWholeNumber)
		valueLabel->SetText(std::to_string((int)val));
	else
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << val;
		valueLabel->SetText(ss.str());
	}
}

void ZGISpinBox::SetVal(float newVal)
{
	val = newVal;
	ValueChanged(this, val);
}

void ZGISpinBox::IncVal(float byVal)
{
	val += byVal;
	ValueChanged(this, val);
}

void ZGISpinBox::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		panel->SetSize(size);
		panel->SetPosition(position);
	}

	if (spinDirection != 0 && clock->GetDiffSeconds() > 0.3)
	{
		val += shiftVal * spinDirection;
		ValueChanged(this, val);
	}

	ZGIWidget::Render(projection);

	panel->Render(projection);
}

void ZGISpinBox::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}

ZGIWidget * ZGISpinBox::HitTest(Vec2D pos)
{
	return panel->HitTest(pos);
}

void ZGISpinBox::ButtonPressed(ZGIButton *b)
{
	if (b == upButton)
	{
		spinDirection = 1;
	}

	if (b == downButton)
	{
		spinDirection = -1;
	}

	clock->TakeClock();
}

void ZGISpinBox::ButtonReleased(ZGIButton *b)
{
	b->GetBrush()->SetBackgroudHue(Color::Clear);

	if (b == upButton)
	{
		val += shiftVal;
		ValueChanged(this, val);
	}

	if (b == downButton)
	{
		val -= shiftVal;
		ValueChanged(this, val);
	}

	spinDirection = 0;
}

void ZGISpinBox::ButtonHovered(ZGIButton *b)
{
	b->GetBrush()->SetBackgroudHue({0.6f,0.6f,0.6f,0.3f});
}

void ZGISpinBox::ButtonStopedBeingHovered(ZGIButton *b)
{
	b->GetBrush()->SetBackgroudHue(Color::Clear);
}
