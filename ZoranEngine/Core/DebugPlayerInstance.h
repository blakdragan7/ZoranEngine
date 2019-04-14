#pragma once
#include <Core\PlayerInstanceBase.h>

class ZoranEngine_EXPORT DebugPlayerInstance : public PlayerInstanceBase
{
protected:
	virtual void ViewWasResized(float w, float y)override;

public:
	DebugPlayerInstance();
	DebugPlayerInstance(CameraBase* camera);
	~DebugPlayerInstance();
};
