#pragma once
/*
*	class PlayerControllerBase
*
*	This class represents the player entirely. Including the gui the player sees, 
*	the input the player give, and the camera the player sees from
*
*/

class CameraBase;
class Quaternion;
class Vector2D;
class Vector3D;
class Vector2I;
class FrameBufferBase;
class ZoranEngine_EXPORT PlayerInstanceBase
{
protected:
	CameraBase * playerCamera;

protected:
	/* Call back for player controllers view frustrum being resized, generally because of a window resize. */
	virtual void ViewWasResized(float w, float h) = 0;

public:
	PlayerInstanceBase();
	PlayerInstanceBase(CameraBase* camera);
	virtual ~PlayerInstanceBase();

	/* Renders the Player camera */
	virtual void RenderPlayer();
	
	void WindowResizedView(float w, float h);
	void WindowResizedView(const Vector2D& size);

	virtual void ScaleView(const Vector3D& scale);
	virtual void ScaleView(const Vector2D& scale);
	virtual void TranslateView(const Vector3D& translation);
	virtual void TranslateView(const Vector2D& translation);
	virtual void RotateView(const Vector3D& eulerRotation);
	virtual void RotateView(const Quaternion& rotation);
	virtual void RotateView(const float rotation);

	virtual void SetCameraSceneBuffer(FrameBufferBase* sceneBuffer);

	inline CameraBase* GetPlayerCamera() { return playerCamera; }
};

