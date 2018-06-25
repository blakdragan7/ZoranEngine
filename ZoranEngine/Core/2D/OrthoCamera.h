#pragma once
#include "Core/2D/Camera2D.h"

class OrthoCamera : public Camera2D
{
private:
	MatrixF orthoModel;

public:
	OrthoCamera(std::string name,double width,double height,double rotation);
	~OrthoCamera();

	virtual MatrixF GetModel()override;
};

