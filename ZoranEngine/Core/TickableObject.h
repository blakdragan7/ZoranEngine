#pragma once

class TickableObject
{
public:
	virtual void Tick(double deltaTime) = 0;
};