#include "stdafx.h"
#include "CameraBase.h"
#include "SceneObject.h"

#include <Rendering/FrameBufferBase.h>

CameraBase::CameraBase() : sceneBuffer(0)
{
}

CameraBase::~CameraBase()
{
	if (sceneBuffer)delete sceneBuffer;
}

void CameraBase::SetSceneBuffer(FrameBufferBase * _sceneBuffer)
{
	if (sceneBuffer)delete sceneBuffer; 
	sceneBuffer = _sceneBuffer;
}

const ImageResource CameraBase::GetCameraTexture()const
{
	if (sceneBuffer)return sceneBuffer->GetTargetTexture();
	return nullptr;
}

void CameraBase::Render()
{
	sceneBuffer->RenderObject(cameraModelCache);
}
