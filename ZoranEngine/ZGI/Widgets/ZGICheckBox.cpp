#include "stdafx.h"
#include "ZGICheckBox.h"

#include <Rendering/TextureBase.h>
#include <Rendering/TextureManager.h>

#include <Interfaces/IZGICheckBoxEventHandler.h>
#include <ZGI/Widgets/ZGIImage.h>

#include <ZGI/Panels/ZGIHorizontalBoxPanel.h>

void ZGICheckBox::WasChecked(bool checked)
{
	if (callback)
	{
		callback(checked);
	}
	if (handler)
	{
		handler->WasChecked(checked);
	}
}

ZGICheckBox::ZGICheckBox(ZGIVirtualWindow* owningWindow) : handler(0), callback(0), content(0), ZGIWidget(owningWindow)
{
	panel = new ZGIHorizontalBoxPanel(owningWindow);
	image = new ZGIImage(owningWindow);
	image->SetImage("emptyBox.png");
	checkImage = tManager->TextureForFilePath("checkImage.png");

	hoverHue.r = 0.6f;
	hoverHue.g = 0.6f;
	hoverHue.b = 0.6f;
	hoverHue.a = 0.3f;

	panel->AddWidget(image);
}

ZGICheckBox::ZGICheckBox(ZGIWidget * content, ZGIVirtualWindow * owningWindow) : content(content), ZGIWidget(owningWindow)
{
	panel = new ZGIHorizontalBoxPanel(owningWindow);
	image = new ZGIImage(owningWindow);
	image->SetImage("emptyBox.png");
	checkImage = tManager->TextureForFilePath("checkImage.png");

	hoverHue.r = 0.6f;
	hoverHue.g = 0.6f;
	hoverHue.b = 0.6f;
	hoverHue.a = 0.3f;

	panel->AddWidget(image);
	panel->AddWidget(content, 6);
}

ZGICheckBox::~ZGICheckBox()
{
	delete panel;
	delete image;
	if (content)delete content;
	tManager->DestroyTexture(checkImage);
}

void ZGICheckBox::SetChecked(bool checked)
{
	if (isChecked != checked)
	{
		isChecked = checked;
		image->SetForegroundImage(isChecked ? checkImage : 0);

		WasChecked(checked);
	}
}

void ZGICheckBox::SetCheckImage(const char * imagePath)
{
	tManager->DestroyTexture(checkImage);
	checkImage = tManager->TextureForFilePath(imagePath);
}

void ZGICheckBox::SetCheckImage(TextureBase * image)
{
	tManager->DestroyTexture(checkImage);
	checkImage = image;
}

void ZGICheckBox::SetContent(ZGIWidget * content)
{
	if (this->content)
	{
		panel->RemoveWidget(this->content);
	}

	this->content = content;
	panel->AddWidget(content, 6);
}

void ZGICheckBox::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		panel->SetSize(size);
		panel->SetPosition(position);
	}

	ZGIWidget::Render(projection);

	panel->Render(projection);
}

ZGIWidget * ZGICheckBox::HitTest(Vec2D pos)
{
	return ZGIWidget::HitTest(pos);
}

void ZGICheckBox::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher Stuff
}

bool ZGICheckBox::MouseUp(const PlatformMouseBase&)
{
	SetChecked(!isChecked);

	return true;
}

bool ZGICheckBox::MouseEnterd(const PlatformMouseBase&)
{
	image->SetForgroundHue(hoverHue);

	return false;
}

bool ZGICheckBox::MouseLeft(const PlatformMouseBase&)
{
	image->SetForgroundHue(Color::Clear);

	return false;
}
