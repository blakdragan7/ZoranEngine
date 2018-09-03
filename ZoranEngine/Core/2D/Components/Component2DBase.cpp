#include "stdafx.h"
#include "Component2DBase.h"


Component2DBase::Component2DBase(ComponentType componentType) : ComponentBase(componentType)
{
}


Component2DBase::~Component2DBase()
{
}

const Vector2D & Component2DBase::GetWorldLocation()
{
	if (parent != 0)
	{
		Matrix44 worldMat = GetWorldMatrix();
		return worldMat * offset;
	}
	else return offset;
}

void Component2DBase::CalculateLocalMatrix()
{
	localMatrix.makeIdentity();

	Matrix44 scaleMat = Matrix44::ScaleMatrix(scale);
	Matrix44 translateMat = Matrix44::TranslationMatrix(offset);
	Matrix44 rotationMat = Matrix44::RotationMatrix(rotation, {0,0,1});

	localMatrix = translateMat * rotationMat * scaleMat;
}

void Component2DBase::SetWorldLocation(float x, float y)
{
	if (parent != 0)
	{
		Vector2D noffset(x, y);

		offset = GetWorldMatrix().inverse() * noffset;
	}
	else
	{
		SetOffset(x, y);
	}
}

void Component2DBase::SetWorldLocation(const Vector2D & offset)
{
	if (parent != 0)
	{
		this->offset = GetWorldMatrix().inverse() * offset;
	}
	else
	{
		SetOffset(offset);
	}
}
