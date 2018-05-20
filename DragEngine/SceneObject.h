#pragma once

#include "Vector3.h"
#include "Quaternion.h"

#include <mutex>
/*
* Very Basic Object, Essentially represents bare minmum needed to be rendered on the scene
*/
class RenderEngineBase;
class ShaderProgramBase;
class RenderedObjectBase;
class DRAGENGINE_EXPORT SceneObject
{
private:
	Vector3D pos;
	Quaternion rotation;
	RenderEngineBase* renderEngine;
	ShaderProgramBase* shaderProgram;
	RenderedObjectBase* renderedObject;

#pragma warning(push)
#pragma warning(disable:4251)
	std::mutex mutex;
#pragma warning(pop)

protected:
	void WaitForMutex();
	void UnlockMutex();

	void SetShaderProgram(ShaderProgramBase* newShaderProgram);
	void SetRenderedObject(RenderedObjectBase* newRenderedObject);

public:
	SceneObject();
	SceneObject(RenderEngineBase* engine) { renderEngine = engine; }
	virtual ~SceneObject() {}

	virtual void PostRender();
	virtual void RenderScene();
	virtual void PreRender();

    // Getter / Setter

	void SetRotation(Vector3D eulor);
	void SetRotationFromAxis(Vector3D axis);
	void SetPosition(Vector3D pos);
	void SetPosition(double x, double y, double z);

	inline RenderEngineBase* GetRenderEngine() { return renderEngine; }
	inline ShaderProgramBase* GetShaderProgram() { return shaderProgram; }
	inline RenderedObjectBase* GetRenderedObject() { return renderedObject; }

	Vector3D GetRotationAsEulor();
	
	inline Quaternion GetRotation() { return rotation; }
	void SetRotation(Quaternion quat);

	void RotateByScaledAxis(Vector3D axis);
	void RotateByQuat(Quaternion quat);
	void RotateByEulor(Vector3D eulor);

	void Translate(Vector3D delta);

	MatrixD GetModel();
};

