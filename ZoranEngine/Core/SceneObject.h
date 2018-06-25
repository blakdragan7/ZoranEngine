#pragma once

#include <mutex>
#include "Core/TickableObject.h"
#include <Math/Matrix.hpp>
/*
* Very Basic Object, Essentially represents bare minmum needed to be rendered on the scene
*/
class RenderEngineBase;
class ShaderProgramBase;
class RenderedObjectBase;
class CollisionObjectBase;
class PhysicsObjectBase;

class ZoranEngine_EXPORT SceneObject : public TickableObject
{
protected:

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

	inline RenderEngineBase* GetRenderEngine() { return renderEngine; }
	inline ShaderProgramBase* GetShaderProgram() { return shaderProgram; }
	inline RenderedObjectBase* GetRenderedObject() { return renderedObject; }
	inline CollisionObjectBase* GetCollision() { return collision; }
	inline PhysicsObjectBase* GetPhysics() { return physicsObject; }

	virtual MatrixF GetModel() = 0;
	virtual MatrixF GetScaleMatrix3x3() = 0;
	virtual MatrixF GetScaleMatrix4x4() = 0;
};

