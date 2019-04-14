#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/RenderTypes.h>
#include <Math/Vector2.h>
#include <functional>

#include <Resources/ImageResource.h>

/*
* This represents an abstraction to off screen rendering. You must subclass the render specific implementation of this class for custom behavior
* For example, subclass OpenGLFrameBuffer for a custom frame buffer implemented with opengl
*/

typedef void (*RenderFunction)(const Matrix44& cameraMatrix);

class TextureBase;
class ZoranEngine_EXPORT FrameBufferBase : public RenderedObjectBase
{
private:
	RenderFunction renderFunction;
	RenderedObjectBase* renderObject;

protected:
	ImageResource targetTexture;
	Vector2I renderSize;
	bool ownsRenderObject;

public:
	FrameBufferBase(Vector2I renderSize, RenderDataType renderType = Render_Data_Type_BGRA_32, RenderDataFormat format = Render_Data_Format_Byte);
	FrameBufferBase(TextureBase* targetTexture);
	virtual ~FrameBufferBase();

	/* Render To Buffer via RenderObject. If gainOwnership is true, delete render object when framebuffer is deleted, if a render function was set it will be unset */
	void SetRenderObject(RenderedObjectBase* renderObject, bool gainOwnerShip);
	/* Render To Buffer via renderFunction, If a render object was previously set it removed */
	void SetRenderFunction(RenderFunction renderFunction);

	inline ImageResource GetTargetTexture()const { return targetTexture; }

	RenderDataFormat GetRenderDataFormat();
	RenderDataType GetRenderDataType();

	virtual void RenderObject(const Matrix44& cameraMatrix)override;
};

