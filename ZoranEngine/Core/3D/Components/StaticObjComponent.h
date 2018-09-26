#pragma once
#include <Core\3D\Components\Visible3DComponent.h>

class ModelRenderer;
class TriangleStripRenderer;
class StaticObjComponent : public Visible3DComponent
{
private:
	ModelRenderer * modelRenderer;
	TriangleStripRenderer* test;

public:
	StaticObjComponent();
	StaticObjComponent(const char* file);
	~StaticObjComponent();

	void LoadFile(const char* file);

	virtual void PreRender()override;
	virtual void Render()override;
	virtual void PostRender()override {}
};

