#include "stdafx.h"
#include "FrameBufferBase.h"
#include "TextureBase.h"
#include "RenderEngineBase.h"
#include "TextureManager.h"

FrameBufferBase::FrameBufferBase(Vector2I renderSize, RenderDataType renderType, RenderDataFormat format) : renderFunction(0), targetTexture(0), renderObject(0),ownsRenderObject(false)
{
	targetTexture = rEngine->CreateTexture(0, renderType,format,renderSize);
}

FrameBufferBase::FrameBufferBase(TextureBase* targetTexture) : renderFunction(0), targetTexture(targetTexture), renderObject(0), ownsRenderObject(false)
{
}

FrameBufferBase::~FrameBufferBase()
{
	if (targetTexture)tManager->DestroyTexture(targetTexture);
	if (ownsRenderObject && renderObject)delete renderObject;
}

void FrameBufferBase::SetRenderObject(RenderedObjectBase* renderObject, bool gainOwnerShip)
{
	if (ownsRenderObject && renderObject)
	{
		delete renderObject;
		renderFunction = 0;
	}

	ownsRenderObject = gainOwnerShip; 
	this->renderObject = renderObject;
}

void FrameBufferBase::SetRenderFunction(RenderFunction renderFunction)
{
	if (ownsRenderObject && renderObject)
	{
		delete renderObject;
		renderObject = 0;
	}

	this->renderFunction = renderFunction;
}

RenderDataFormat FrameBufferBase::GetRenderDataFormat()
{
	return targetTexture->GetRenderDataFormat();
}

RenderDataType FrameBufferBase::GetRenderDataType()
{
	return targetTexture->GetRenderDataType();
}

void FrameBufferBase::RenderObject(const Matrix44& cameraMatrix)
{
	if (renderObject)renderObject->RenderObject(cameraMatrix);
	else if (renderFunction)renderFunction(cameraMatrix);
}
