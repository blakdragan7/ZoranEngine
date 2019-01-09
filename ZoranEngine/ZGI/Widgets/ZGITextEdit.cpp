#include "stdafx.h"
#include "ZGITextEdit.h"
#include <Core/CommomTypes.h>
#include <Core/Resources/FontResource.h>
#include <ZGI/Core/ZGIBrush.h>
#include <ZGI/Widgets/ZGICursor.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <Rendering/Renderers/FontRenderer.h>

#include <Core/PlatformMouseBase.h>

void ZGITextEdit::UpdateCursorFromPos()
{
	if (cursorPos == -1)
	{
		TextGlyph* glyph = renderer->GlyphForPos(0);
		if (glyph)
		{
			cursor->SetPosition(glyph->baseline);
		}
	}
	else
	{
		TextGlyph* glyph = renderer->GlyphForPos(cursorPos);
		if (glyph)
		{
			cursor->SetPosition({ glyph->endline.x, glyph->baseline.y });
		}
	}
}

ZGITextEdit::ZGITextEdit(ZGIVirtualWindow* owningWindow) : needsCursorUpdate(false), cursorPos(-1), ZGILabel(owningWindow)
{
	cursor = new ZGICursor(owningWindow);
}

ZGITextEdit::~ZGITextEdit()
{
	delete cursor;
}

void ZGITextEdit::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
		cursor->SetSize({1.0f,renderer->GetPPTSize() * 0.7f});
	}

	ZGILabel::Render(projection);

	if (needsCursorUpdate)
	{
		needsCursorUpdate = false;
		UpdateCursorFromPos();
	}

	if (isInFocus)cursor->Render(projection);
}

void ZGITextEdit::Animate(float dt)
{
	if(isInFocus)cursor->Animate(dt);

	ZGILabel::Animate(dt);
}

bool ZGITextEdit::RawKeyEvent(KeyEventType type, unsigned uni)
{
	if (type == KeyEventType_Key_Down)
	{
		switch(uni)
		{
		case Key_Delete:
			renderer->RemoveGlyphAtCursurPos(cursorPos+1);
			break;
		case Key_BackSpace:
			renderer->RemoveGlyphAtCursurPos(cursorPos);
			if (cursorPos != -1)
			{
				cursorPos--;
				needsCursorUpdate = true;
			}
			break;
		case Key_Left_Arrow:
			cursorPos--;
			if (cursorPos < -1)cursorPos = -1;
			needsCursorUpdate = true;
			cursor->SetTempSolid();
			break;
		case Key_Right_Arrow:
			cursorPos++;
			needsCursorUpdate = true;
			cursor->SetTempSolid();
			break;
		case Key_Up_Arrow:
			cursorPos = renderer->CursorPosInAboveLine(cursorPos);
			needsCursorUpdate = true;
			cursor->SetTempSolid();
			break;
		case Key_Down_Arrow:
			cursorPos = renderer->CursorPosInBelowLine(cursorPos);
			needsCursorUpdate = true;
			cursor->SetTempSolid();
			break;
		}
	}
	return false;
}

bool ZGITextEdit::CharEvent(unsigned uni)
{
	renderer->InsertGlyph(uni, cursorPos + 1);

	cursorPos++;

	needsCursorUpdate = true;
	cursor->SetTempSolid();
	return true;
}

bool ZGITextEdit::MouseUp(const PlatformMouseBase & mouse)
{
	owningWindow->SetFirstResponder(this);

	Vec2D pos = owningWindow->ConvertAbsoluteToVirtual(mouse.GetPosition());
	cursorPos = renderer->CursorPosForLocation(pos);

	needsCursorUpdate = true;

	return true;
}
