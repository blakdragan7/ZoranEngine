#include "stdafx.h"
#include "Component3DBase.h"


Component3DBase::Component3DBase(ComponentType componentType) : ComponentBase(componentType)
{
}


Component3DBase::~Component3DBase()
{
}

void Component3DBase::CalculateLocalMatrix()
{
	localMatrix.makeIdentity();

	Matrix44 scaleMat = Matrix44::ScaleMatrix(scale);
	Matrix44 translateMat = Matrix44::TranslationMatrix(offset);
	Matrix44 rotationMat = rotation.AsRotationMatrix();

	localMatrix = translateMat * rotationMat * scaleMat;
}
