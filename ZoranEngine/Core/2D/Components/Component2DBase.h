#pragma once
#include <Core\Components\ComponentBase.h>
#include <Math/Vector2.h>

class ZoranEngine_EXPORT Component2DBase : public ComponentBase
{
private:
	Vector2D offset;
	Vector2D scale;

	float rotation;

public:
	Component2DBase();
	~Component2DBase();

	// mutators

	inline void Scale(const Vector2D& scale) { this->scale *= scale; }
	inline void SetScale(const Vector2D& scale) { this->scale = scale; }

	inline void Translate(const Vector2D& translation) { this->offset += translation; }
	inline void SetOffset(const Vector2D& offset) { this->offset = offset; }

	inline void Rotate(float rad) { rotation += rad; }
	inline void RotateByDegree(float degree) { rotation += ((degree * 3.14159265359) / 180.0f); }

	// accessors

	inline const Vector2D& GetScale()const { return scale; }
	inline const Vector2D& GetOffset()const { return offset; }
	inline float GetRotation()const { return rotation; }
};

