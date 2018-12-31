#include "stdafx.h"
#include "ZGIBasicSocket.h"


ZGIBasicSocket::ZGIBasicSocket(ZGIPanel* owningPanel) : owningPanel(owningPanel), content(0), alignment(0), 
widthPercent(0.0f), heightPercent(0.0f), width(1.0f), height(1.0f)
{
}

ZGIBasicSocket::ZGIBasicSocket(ZGIWidget * content, ZGIPanel* owningPanel) : owningPanel(owningPanel), content(content), 
alignment(0), widthPercent(0.0f), heightPercent(0.0f), width(1.0f), height(1.0f)
{
}

ZGIBasicSocket::ZGIBasicSocket(ZGIWidget * content, AlignmentBit alignment, ZGIPanel* owningPanel) : owningPanel(owningPanel), 
content(content), alignment(alignment), widthPercent(0.0f), heightPercent(0.0f), width(1.0f), height(1.0f)
{
}

ZGIBasicSocket::~ZGIBasicSocket()
{
}
