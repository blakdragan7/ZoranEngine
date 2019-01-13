#pragma once

class ITickableObject
{
public:
	virtual void Tick(float deltaTime) = 0;
};