#pragma once
#include <Core\Components\ComponentBase.h>
#include <Math/Vector3.h>
#include <Math/Quaternion.h>

class ZoranEngine_EXPORT Component3DBase : public ComponentBase
{
private:
	Vector3D offset;
	Vector3D scale;

	Vector3D startingSize;

	Quaternion rotation;
	
public:
	Component3DBase();
	~Component3DBase();

	// mutators

	inline void Scale(float x,float y,float z) { this->scale.x *= x; this->scale.y *= y; this->scale.z *= z; isDirty = true; }
	inline void Scale(const Vector3D& scale) { this->scale *= scale; isDirty = true;}

	inline void SetScale(float x, float y, float z) { this->scale.x = x; this->scale.y = y; this->scale.z = z; isDirty = true; }
	inline void SetScale(const Vector3D& scale) { this->scale = scale; isDirty = true;}

	inline void Translate(float x, float y, float z) { this->offset.x += x; this->offset.y += y; this->offset.z += z; isDirty = true; }
	inline void Translate(const Vector3D& translation) { this->offset += translation; isDirty = true;}

	inline void SetOffset(float x, float y, float z) { this->offset.x = x; this->offset.y = y; this->offset.z = z; isDirty = true; }
	inline void SetOffset(const Vector3D& offset) { this->offset = offset; isDirty = true;}

	inline void Rotate(const Quaternion& quat) { rotation = quat * rotation; isDirty = true;}
	inline void Rotate(const Vector3D& euler) { rotation = Quaternion::FromEuler(euler) * rotation; isDirty = true;}
	inline void RotateByScaledAxis(const Vector3D& axis) { rotation = Quaternion::FromScaledAxis(axis) * rotation; isDirty = true;}

	inline void SetRotation(const Quaternion& quat) { rotation = quat; isDirty = true; }
	inline void SetRotationFromScaleAxis(const Vector3D& axis) { rotation = Quaternion::FromScaledAxis(axis); isDirty = true;}
	inline void SetRotationFromEuler(const Vector3D& euler) { rotation = Quaternion::FromEuler(euler); isDirty = true;}
	inline void SetRotationFromEuler(float yaw, float pitch, float roll) { rotation = Quaternion::FromEuler(yaw, pitch, roll); isDirty = true;}

	inline void SetStartingSize(const Vector3D& size) { startingSize = size; isDirty = true;}
	inline void SetStartingSize(float x,float y,float z) { startingSize.x = x; startingSize.y = y;startingSize.z = z; isDirty = true;}

	inline void SetSize(const Vector3D& size) { startingSize = size / scale; isDirty = true;}
	inline void SetSize(float x, float y, float z) { startingSize.x = x / scale.x; startingSize.y = y / scale.y;startingSize.z = z / scale.z; isDirty = true;}
	
	// accessors

	inline const Vector3D& GetScale()const { return scale; }
	inline const Vector3D& GetOffset()const { return offset; }
	inline const Quaternion& GetRotation()const { return rotation; }
	inline const Vector3D GetSize()const { return startingSize * scale; }

	void SetWorldLocation(const Vector3D& location);
	const Vector3D GetWorldLocation();
	virtual void CalculateLocalMatrix()override;
};

