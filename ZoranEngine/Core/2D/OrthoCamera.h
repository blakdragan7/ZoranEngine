#pragma once
#include "Core/2D/Camera2D.h"

class OrthoCamera : public Camera2D
{
private:
	MatrixF orthoModel;

public:
	OrthoCamera(std::string name,float width,float height,float rotation);
	~OrthoCamera();

	virtual MatrixF GetModel()override;
};

