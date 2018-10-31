#include "stdafx.h"
#include "PlatformMouseBase.h"

PlatformMouseBase::PlatformMouseBase() : isExplicit(false), isVisible(false), leftMouseIsPressed(false), middleMouseIsPressed(false), rightMouseIsPressed(false)
{
	memset(otherButtons, 0, sizeof(otherButtons));
}

PlatformMouseBase::PlatformMouseBase(Vec2D pos) : position(pos), isExplicit(false), isVisible(false), leftMouseIsPressed(false), \
	middleMouseIsPressed(false), rightMouseIsPressed(false) 
{ 
	memset(otherButtons, 0, sizeof(otherButtons)); 
}