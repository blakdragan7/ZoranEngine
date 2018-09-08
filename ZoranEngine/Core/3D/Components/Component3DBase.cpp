#include "stdafx.h"
#include "Component3DBase.h"


Component3DBase::Component3DBase()
{
}


Component3DBase::~Component3DBase()
{
}

void Component3DBase::SetWorldLocation(const Vector3D & location)
{
	if (parent)
		offset = GetWorldMatrix().inverse() * location;
	else
		offset = location;
}

const Vector3D Component3DBase::GetWorldLocation()
{
	if (parent)
	{
		return GetWorldMatrix() * offset;
	}
	else
	{
		return offset;
	}
}

void Component3DBase::CalculateLocalMatrix()
{
	localMatrix.makeIdentity();

	Matrix44 scaleMat = Matrix44::ScaleMatrix(scale);
	Matrix44 translateMat = Matrix44::TranslationMatrix(offset);
	Matrix44 rotationMat = rotation.AsRotationMatrix();

	localMatrix = translateMat * rotationMat * scaleMat;
}
