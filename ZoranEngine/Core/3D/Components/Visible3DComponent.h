#pragma once
#include <Core\Components\VisibleComponentBase.h>
#include <Core/3D/Components/Component3DBase.h>

class ZoranEngine_EXPORT Visible3DComponent : public VisibleComponentBase, public Component3DBase
{
public:
	Visible3DComponent();
	~Visible3DComponent();
};

