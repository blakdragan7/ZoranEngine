#include "stdafx.h"
#include "PlayerInstanceBase.h"

#include <Core/CameraBase.h>

PlayerInstanceBase::PlayerInstanceBase() : playerCamera(0)/*, playerGUI(0)*/
{
}

PlayerInstanceBase::PlayerInstanceBase(CameraBase * camera/*, SomGuiClass* gui*/) : playerCamera(camera)/*, playerGUI(gui)*/
{
}

PlayerInstanceBase::~PlayerInstanceBase()
{
	if (playerCamera)delete playerCamera;
	/*if(playerGUI)delete playerGUI;*/
}

void PlayerInstanceBase::RenderPlayer()
{
	playerCamera->Render();
	/*playerGUI->Render();*/
}

void PlayerInstanceBase::WindowResizedView(float w, float h)
{
	playerCamera->ScreenResized(w, h);

	ViewWasResized(w, h);
}

void PlayerInstanceBase::WindowResizedView(const Vector2D & size)
{
	playerCamera->ScreenResized(size);

	ViewWasResized(size.w, size.h);
}

void PlayerInstanceBase::ScaleView(const Vector3D& scale)
{
	playerCamera->Scale(scale);
}

void PlayerInstanceBase::ScaleView(const Vector2D& scale)
{
	playerCamera->Scale(scale);
}

void PlayerInstanceBase::TranslateView(const Vector3D& translation)
{
	playerCamera->Translate(translation);
}

void PlayerInstanceBase::TranslateView(const Vector2D& translation)
{
	playerCamera->Translate(translation);
}

void PlayerInstanceBase::RotateView(const Vector3D& eulerRotation)
{
	playerCamera->Rotate(eulerRotation);
}

void PlayerInstanceBase::RotateView(const Quaternion& rotation)
{
	playerCamera->Rotate(rotation);
}

void PlayerInstanceBase::RotateView(const float rotation)
{
	playerCamera->Rotate(rotation);
}

void PlayerInstanceBase::SetCameraSceneBuffer(FrameBufferBase * sceneBuffer)
{
	playerCamera->SetSceneBuffer(sceneBuffer);
}
