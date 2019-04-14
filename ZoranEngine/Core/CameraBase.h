#pragma once
#include <Math/Matrix44.h>
#include <Math/Quaternion.h>
#include <Math/Vector2.h>
#include <string>

#include <Resources/ImageResource.h>

class SceneObject;
class FrameBufferBase;
class TextureBase;
class ZoranEngine_EXPORT CameraBase
{
protected:
	Vector2D screenSize;
	Vector2D cameraExtents;
	Vector2D halfCameraExtents;

	Matrix44 cameraModelCache;
	FrameBufferBase* sceneBuffer;
	
public:
	CameraBase();
	virtual ~CameraBase();
	/*Sets the scene buffer and deletes the current scene buffer if it exists*/
	void SetSceneBuffer(FrameBufferBase* _sceneBuffer);
	/* Returns the Camera Model Matrix */
	inline const Matrix44& GetModel()const { return cameraModelCache; }

	virtual void Translate(float deltax, float deltay, float deltaz) = 0;
	virtual void Translate(Vec2D delta) = 0;
	virtual void Translate(Vec3D delta) = 0;
	virtual void Rotate(float rotx, float roty, float rotz) = 0;
	virtual void Rotate(float rotation) = 0;
	virtual void Rotate(Vec3D euler) = 0;
	virtual void Rotate(Quat quat) = 0;
	virtual void Scale(Vec3D scale) = 0;
	virtual void Scale(Vec2D scale) = 0;

	virtual Vector2D GetCameraViewingExtentsAtZ(float z) = 0;
	virtual void ScreenResized(float screenWidth, float screenHeight) = 0;
	virtual void ScreenResized(Vec2D Size) = 0;

	virtual void CalculateModelCache() = 0;
	virtual const ImageResource GetCameraTexture()const;

	virtual void Render();
};

