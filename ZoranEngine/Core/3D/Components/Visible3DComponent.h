#pragma once
#include <Core\Components\VisibleComponentBase.h>
#include <Core/3D/Components/Component3DBase.h>

class RenderEngine3DBase;
class ZoranEngine_EXPORT Visible3DComponent : public VisibleComponentBase, public Component3DBase
{
protected:
	RenderEngine3DBase * engine3D;

public:
	Visible3DComponent(RenderEngine3DBase * engine3D = 0);
	~Visible3DComponent();

	virtual  void PreRender()override;

	virtual void SetShaderProgram(ShaderProgramBase* program)override;
};

