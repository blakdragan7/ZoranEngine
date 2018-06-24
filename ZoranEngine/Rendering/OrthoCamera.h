#pragma once
#include "Core/CameraBase.h"

class OrthoCamera : public CameraBase
{
private:
	MatrixF orthoModel;

public:
	OrthoCamera(std::string name,double width,double height,double rotation);
	~OrthoCamera();

	virtual MatrixF GetModel()override;
};

