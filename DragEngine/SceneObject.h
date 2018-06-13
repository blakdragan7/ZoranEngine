#pragma once

#include "Vector3.h"
#include "Quaternion.h"
#include <mutex>

#include "TickableObject.h"

/*
* Very Basic Object, Essentially represents bare minmum needed to be rendered on the scene
*/
class RenderEngineBase;
class ShaderProgramBase;
class RenderedObjectBase;
class CollisionObjectBase;
class PhysicsObjectBase;

class DRAGENGINE_EXPORT SceneObject : public TickableObject
{
private:
	// used to make GetModel more effecient
	MatrixF model;

	Vector3D scale;
	Vector3D pos;
	Quaternion rotation;

	RenderEngineBase* renderEngine;
	ShaderProgramBase* shaderProgram;
	RenderedObjectBase* renderedObject;
	PhysicsObjectBase* physicsObject;

	unsigned long long ID;

#pragma warning(push)
#pragma warning(disable:4251)
	std::mutex mutex;
#pragma warning(pop)

protected:
	CollisionObjectBase * collision;
	bool hasCollision;

public:
#pragma warning(push)
#pragma warning(disable:4251)
	std::string readableName;
#pragma warning(pop)

	bool willEverTick;

protected:
	void WaitForMutex();
	void UnlockMutex();

	void SetShaderProgram(ShaderProgramBase* newShaderProgram);
	void SetRenderedObject(RenderedObjectBase* newRenderedObject);

public:
	SceneObject(std::string);
	SceneObject(std::string, RenderEngineBase* engine);
	virtual ~SceneObject();

	virtual void PostRender();
	virtual void RenderScene();
	virtual void PreRender();

	// Destroys this object removeing it from any part of the engine that it needs to
	virtual void Destroy();

	// default impl
	virtual void Tick(double deltaTime)override {}

    // Getter / Setter

	void SetRotation(Vector3D eulor);
	void SetRotationFromAxis(Vector3D axis);
	void TrySetPosition(Vector3D pos);
	void SetPosition(Vector3D pos);
	void SetPosition(double x, double y, double z);
	void SetScale(Vector3D scale);
	void SetScale(double x, double y, double z);

	Vector3D GetPosition();
	Vector3D GetScale();

	inline RenderEngineBase* GetRenderEngine() { return renderEngine; }
	inline ShaderProgramBase* GetShaderProgram() { return shaderProgram; }
	inline RenderedObjectBase* GetRenderedObject() { return renderedObject; }
	inline CollisionObjectBase* GetCollision() { return collision; }
	inline PhysicsObjectBase* GetPhysics() { return physicsObject; }

	Vector3D GetRotationAsEulor();
	
	inline Quaternion GetRotation() { return rotation; }
	void SetRotation(Quaternion quat);

	void RotateByScaledAxis(Vector3D axis);
	void RotateByQuat(Quaternion quat);
	void RotateByEulor(Vector3D eulor);

	void Translate(Vector3D delta);
	void Scale(Vector3D scale);

	MatrixF GetModel();
	MatrixF GetScaleMatrix3x3();
	MatrixF GetScaleMatrix4x4();

	double DistanceTo(Vector3D pos);
	double DistanceTo(SceneObject* other);
};

