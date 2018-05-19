#pragma once
#include "PlatformTypes.h"
#include "Vector3.h"
#include "Quaternion.h"

#include <mutex>
/*
* Very Basic Object, Essentially represents bare minmum needed to be rendered on the scene
*/
class RenderEngineBase;
class ShaderProgramBase;
class DRAGENGINE_EXPORT SceneObject
{
private:
	Vector3D pos;
	Quaternion rotation;
	RenderEngineBase* renderEngine;
	ShaderProgramBase* shaderProgram;

#pragma warning(push)
#pragma warning(disable:4251)
	std::mutex mutex;
#pragma warning(pop)

protected:
	void WaitForMutex();
	void UnlockMutex();

public:
	SceneObject();
	SceneObject(RenderEngineBase* engine) { renderEngine = engine; }
	virtual ~SceneObject() {}

	virtual void PostRender() = 0;
	virtual void RenderScene() = 0;
	virtual void PreRender() = 0;

    // Getter / Setter

	void SetRotation(Vector3D eulor);
	void SetRotationFromAxis(Vector3D axis);
	void SetPosition(Vector3D pos);
	void SetPosition(double x, double y, double z);

	inline RenderEngineBase* GetRenderEngine() { return renderEngine; }
	inline ShaderProgramBase* GetShaderProgram() { return shaderProgram; }

	Vector3D GetRotationAsEulor();
	
	inline Quaternion GetRotation() { return rotation; }
	void SetRotation(Quaternion quat);

	void RotateByScaledAxis(Vector3D axis);
	void RotateByQuat(Quaternion quat);
	void RotateByEulor(Vector3D eulor);

	void Translate(Vector3D delta);

	MatrixD GetModel();
};

