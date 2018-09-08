#pragma once
#include <Core\SceneObject.h>
#include <Math/Vector3.h>
#include <Math/Quaternion.h>

#include <Core/3D/Components/Component3DBase.h>
/*
*  3D scene objects scene objects who's coordinates are represented with 3D vectors and a quatornian for rotation
*/

class ZoranEngine_EXPORT SceneObject3D : public SceneObject
{
protected:
	Component3DBase * root3DComponent;

public:
	SceneObject3D(std::string);
	SceneObject3D(Component3DBase * root3DComponent,std::string);
	virtual ~SceneObject3D();

	// Destroys this object removeing it from any part of the engine that it needs to
	virtual void Destroy()override;

	// Getter / Setter

	void SetRotation(const Vector3D& eulor);
	void SetRotationFromAxis(const Vector3D& axis);

	void SetPosition(const Vector3D& pos);
	void SetPosition(float x, float y, float z);

	void SetScale(const Vector3D& scale);
	void SetScale(float x, float y, float z);

	void SetSize(const Vector3D& size);
	void SetSize(float x,float y,float z);

	Vector3D GetRotationAsEulor()const;

	void SetRotation(const Quaternion& quat);

	void RotateByQuat(const Quaternion& quat);
	void RotateByEulor(const Vector3D& eulor);
	void RotateByScaledAxis(const Vector3D& axis);

	void Translate(const Vector3D& delta);
	void Scale(const Vector3D& scale);

	virtual void PreCaclModel()override;
	virtual Matrix44 GetScaleMatrix4x4()override;

	inline Quaternion GetRotation()const { return root3DComponent->GetRotation(); }
	inline const Vector3D& GetPosition()const { return root3DComponent->GetOffset(); };
	inline const Vector3D& GetScale()const { return root3DComponent->GetScale(); };
	inline const Vector3D GetSize()const { return root3DComponent->GetSize(); }

	float DistanceTo(const Vector3D& pos);
	float DistanceTo(SceneObject3D* other);
};

