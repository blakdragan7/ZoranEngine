#pragma once
#include <Core\Components\ComponentBase.h>
#include <Math/Vector3.h>
#include <Math/Quaternion.h>

class ZoranEngine_EXPORT Component3DBase : public ComponentBase
{
private:
	Vector3D offset;
	Vector3D scale;

	Quaternion rotation;
	
public:
	Component3DBase();
	~Component3DBase();

	// mutators

	inline void Scale(const Vector3D& scale) { this->scale *= scale; }
	inline void SetScale(const Vector3D& scale) { this->scale = scale; }

	inline void Translate(const Vector3D& translation) { this->offset += translation; }
	inline void SetOffset(const Vector3D& offset) { this->offset = offset; }

	inline void Rotate(const Quaternion& quat) { rotation = quat * rotation; }
	inline void Rotate(const Vector3D& eulor) { rotation = Quaternion::FromEuler(eulor) * rotation; }
	inline void RotateByScaledAxis(const Vector3D& axis) { rotation = Quaternion::FromScaledAxis(axis) * rotation; }

	// accessors

	inline const Vector3D& GetScale()const { return scale; }
	inline const Vector3D& GetOffset()const { return offset; }
	inline const Quaternion& GetRotation()const { return rotation; }
};

