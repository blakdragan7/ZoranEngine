#include "stdafx.h"
#include "OrthoCamera.h"
#include "Core/SceneObject.h"

OrthoCamera::OrthoCamera(std::string name, double width, double height, double rotation) : CameraBase(name, Vec3D(0,0,0), Vec3D(1,1,1), Vec3D(0,0,0))
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
	MatrixF sceneModel = GetSceneObject()->GetModel();

	return orthoModel * sceneModel;
}
