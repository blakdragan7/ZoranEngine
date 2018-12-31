#pragma once
#include <Core/CommomTypes.h>

class ZGIPanel;
class ZGIWidget;
class ZGIBasicSocket
{
public:
	ZGIWidget* content;
	AlignmentBit alignment;

	ZGIPanel* owningPanel;

	float width;
	float height;

	float widthPercent;
	float heightPercent;

public:
	ZGIBasicSocket(ZGIPanel* owningPanel);
	ZGIBasicSocket(ZGIWidget* content, ZGIPanel* owningPanel);
	ZGIBasicSocket(ZGIWidget* content, AlignmentBit alignment, ZGIPanel* owningPanel);
	~ZGIBasicSocket();

	inline bool operator==(const ZGIBasicSocket& other)const
	{
		return content == other.content;
	}

	inline bool operator==(const ZGIWidget* widget)const
	{
		return content == widget;
	}

	inline bool operator!=(const ZGIBasicSocket& other)const
	{
		return content != other.content;
	}

	inline bool operator!=(const ZGIWidget* widget)const
	{
		return content != widget;
	}
};

