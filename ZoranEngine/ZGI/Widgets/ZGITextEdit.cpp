#include "stdafx.h"
#include "ZGITextEdit.h"
#include <Core/CommomTypes.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <Rendering/Renderers/FontRenderer.h>

ZGITextEdit::ZGITextEdit(ZGIVirtualWindow* owningWindow) : ZGILabel(owningWindow)
{
}

ZGITextEdit::~ZGITextEdit()
{
}

bool ZGITextEdit::RawKeyEvent(KeyEventType type, unsigned uni)
{
	if (type == KeyEventType_Key_Down)
	{
		if (uni == Key_BackSpace)
		{
			renderer->RemoveGlyphAtCursur();
		}
	}
	return false;
}

bool ZGITextEdit::CharEvent(unsigned uni)
{
	renderer->AddGlyph(uni);

	return true;
}

bool ZGITextEdit::MouseUp(const PlatformMouseBase *)
{
	owningWindow->SetFirstResponder(this);

	return true;
}
