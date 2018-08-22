#pragma once
#include <Core\SceneObject.h>
#include <Math/Vector3.h>
#include <Math/Matrix44.hpp>
#include <Math/Quaternion.h>

class CollisionObjectBase;
class CollisionObject3DBase;
class PhysicsObject3DBase;
class PhysicsObjectBase;

class ZoranEngine_EXPORT SceneObject3D : public SceneObject
{
	// used to make GetModel more effecient
	Matrix44 model;

	Vector3D scale;
	Vector3D pos;
	Quaternion rotation;

protected:
	CollisionObject3DBase * collision;
	PhysicsObject3DBase* physicsObject;


public:
	SceneObject3D(std::string);
	SceneObject3D(std::string, RenderEngineBase* engine);
	virtual ~SceneObject3D();

	virtual void PostRender()override;
	virtual void RenderScene()override;
	virtual void PreRender()override;

	// Destroys this object removeing it from any part of the engine that it needs to
	virtual void Destroy()override;

	// Getter / Setter

	void SetRotation(Vector3D eulor);
	void SetRotationFromAxis(Vector3D axis);
	void SetPosition(Vector3D pos);
	void SetPosition(float x, float y, float z);
	void SetScale(Vector3D scale);
	void SetScale(float x, float y, float z);

	Vector3D GetPosition()const;
	Vector3D GetVelocity()const;
	Vector3D GetScale()const;

	Vector3D GetRotationAsEulor()const;

	inline Quaternion GetRotation()const { return rotation; }
	void SetRotation(Quaternion quat);

	void RotateByScaledAxis(Vector3D axis);
	void RotateByQuat(Quaternion quat);
	void RotateByEulor(Vector3D eulor);

	void Translate(Vector3D delta);
	void Scale(Vector3D scale);

	virtual void PreCaclModel()override;
	//virtual Matrix44 GetScaleMatrix3x3()override;
	virtual Matrix44 GetScaleMatrix4x4()override;

	float DistanceTo(Vector3D pos);
	float DistanceTo(SceneObject3D* other);

	inline CollisionObject3DBase* GetCollision3D() { return collision; }
	inline PhysicsObject3DBase* GetPhysics3D() { return physicsObject; }

	inline CollisionObjectBase* GetCollision() { return (CollisionObjectBase*)collision; }
	inline PhysicsObjectBase* GetPhysics() { return (PhysicsObjectBase*)physicsObject; }
};

