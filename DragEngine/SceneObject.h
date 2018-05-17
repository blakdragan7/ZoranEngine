#pragma once

#include "Vector3.h"
#include "Quaternion.h"
/*
* Very Basic Object, Essentially represents bare minmum needed to be rendered on the scene
*/
class SceneObject
{
private:
	Vector3D pos;
	Quaternion rotation;

public:
	SceneObject() {}
	virtual ~SceneObject() {}

	virtual void PostRender() = 0;
	virtual void RenderScene() = 0;
	virtual void PreRender() = 0;

    // Getter / Setter

	void SetRotation(Vector3D eulor);
	void SetRotationFromAxis(Vector3D axis);
	void SetPosition(Vector3D pos);
	void SetPosition(double x, double y, double z);

	Vector3D GetRotationAsEulor();
	
	inline Quaternion GetRotation() { return rotation; }
	inline void SetRotation(Quaternion quat) { rotation = quat; }

	void RotateByScaledAxis(Vector3D axis);
	void RotateByQuat(Quaternion quat);
	void RotateByEulor(Vector3D eulor);

	void Translate(Vector3D delta);

	Mat4D GetModel();
};

