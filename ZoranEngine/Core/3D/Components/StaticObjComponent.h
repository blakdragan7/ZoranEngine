#pragma once
#include <Core\3D\Components\Visible3DComponent.h>

#include <Resources/ModelResource.h>

class TriangleStripRenderer;
class StaticObjComponent : public Visible3DComponent
{
private:
	ModelResource model;
	TriangleStripRenderer* test;

public:
	StaticObjComponent();
	StaticObjComponent(const char* file);
	StaticObjComponent(ModelResource model);
	~StaticObjComponent();

	void LoadFile(const char* file);

	virtual void PreRender()override;
	virtual void Render(const Matrix44& cameraMatrix)override;
	virtual void PostRender()override {}
};

