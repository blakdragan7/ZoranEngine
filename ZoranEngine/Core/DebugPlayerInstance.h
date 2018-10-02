#pragma once
#include <Core\PlayerInstanceBase.h>

class DebugPlayerInstance : public PlayerInstanceBase
{
protected:
	virtual void ViewWasResized(float w, float y)override;

public:
	DebugPlayerInstance();
	DebugPlayerInstance(CameraBase* camera/*, SomGuiClass* gui*/);
	~DebugPlayerInstance();
};
