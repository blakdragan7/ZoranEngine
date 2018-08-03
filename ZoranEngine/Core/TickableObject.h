#pragma once

class TickableObject
{
public:
	virtual void Tick(float deltaTime) = 0;
};