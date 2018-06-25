#include "stdafx.h"
#include "OrthoCamera.h"
#include "Core/SceneObject.h"

OrthoCamera::OrthoCamera(std::string name, double width, double height, double rotation) : Camera2D(name,Vec2D(0,0), Vec2D(1,1), 0)
{
	double width_2 = width / 2.0;
	double height_2 = height / 2.0;
	orthoModel = MatrixF::GLOrthoMatrix(-width_2,-height_2,width_2,height_2,-1,10);
}

OrthoCamera::~OrthoCamera()
{
}

MatrixF OrthoCamera::GetModel()
{
	MatrixF sceneModel = Camera2D::GetModel();

	return orthoModel * sceneModel;
}
