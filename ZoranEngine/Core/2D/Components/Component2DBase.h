#pragma once
#include <Core\Components\ComponentBase.h>
#include <Math/Vector2.h>

class ZoranEngine_EXPORT Component2DBase : public ComponentBase
{
private:
	Vector2D offset;
	Vector2D scale;
	
	Vector2D startingSize;

	float rotation;

public:
	Component2DBase(ComponentType componentType);
	~Component2DBase();

	// mutators
	inline void SetStartingSize(float x, float y) { startingSize.x = x; startingSize.y = y; isDirty = true;}
	inline void SetStartingSize(const Vector2D& size) { startingSize = size; isDirty = true;}

	inline void SetSize(float x, float y) { startingSize.x = (x / scale.x); startingSize.y = (y / scale.y); isDirty = true;}
	inline void SetSize(const Vector2D& size) { startingSize = size / scale; isDirty = true;}

	inline void Scale(const Vector2D& scale) { this->scale *= scale; isDirty = true; }
	inline void Scale(float x, float y) { this->scale.x *= x; this->scale.y *= y; isDirty = true; }

	inline void SetScale(const Vector2D& scale) { this->scale = scale; isDirty = true; }
	inline void SetScale(float x, float y) { this->scale.x = x; this->scale.y = y; isDirty = true; }

	inline void Translate(float x, float y) { this->offset.x += x; this->offset.y += y; isDirty = true; }
	inline void Translate(const Vector2D& translation) { this->offset += translation; isDirty = true;}

	inline void SetOffset(float x, float y) { this->offset.x = x; this->offset.y = y; isDirty = true; }
	inline void SetOffset(const Vector2D& offset) { this->offset = offset; isDirty = true; }

	inline void Rotate(float rad) { rotation += rad; isDirty = true;}
	inline void RotateByDegree(float degree) { rotation += ((degree * 3.14159265359f) / 180.0f); isDirty = true;}
	inline void SetRotatation(float rotation) { this->rotation = rotation; isDirty = true; }
	inline void SetRotatationDegree(float degree) { this->rotation = ((degree * 3.14159265359f) / 180.0f); isDirty = true; }

	// accessors

	inline const Vector2D& GetScale()const { return scale; }
	inline const Vector2D& GetOffset()const { return offset; }

	inline const float& GetRotation()const { return rotation; }

	inline const Vec2D& GetSize()const { return startingSize * scale; }

	const Vector2D& GetWorldLocation();

	virtual void CalculateLocalMatrix()override;

	void SetWorldLocation(float x, float y);
	void SetWorldLocation(const Vector2D& offset);
};

