#pragma once
#include <Math/Vector2.h>

/*  
	Mouse Instance 

	Mouse Instance is used to track the mouse information on the window
*/

class PlatformMouseBase
{
private:
	Vector2D position;
	Vector2D deltaPosition;
	/* Mouse Flags */
	unsigned isExplicit : 1;
	unsigned isVisible : 1;

	unsigned leftMouseIsPressed : 1;
	unsigned middleMouseIsPressed : 1;
	unsigned rightMouseIsPressed : 1;

	struct{
		unsigned isPushed : 1;
	}otherButtons[15];

public:
	PlatformMouseBase();
	PlatformMouseBase(Vec2D pos);
	virtual ~PlatformMouseBase() {}

	inline void SetPosition(Vec2D pos) { deltaPosition = (pos - position); position = pos; }
	inline Vec2D GetPosition()const { return position; }

	inline Vec2D GetDelta()const { return deltaPosition; }

	inline void SetIsExplicit(bool isExplicit) { this->isExplicit = isExplicit; }
	inline void SetIsVisisble(bool isVisible) { this->isVisible = isVisible; }

	inline void SetLeftMouseIsPressed(bool isPressed) { leftMouseIsPressed = isPressed; }
	inline void SetMiddleMouseIsPressed(bool isPressed) { middleMouseIsPressed = isPressed; }
	inline void SetRightMouseIsPressed(bool isPressed) { rightMouseIsPressed = isPressed; }

	inline void SetOtherButtonIsPressed(unsigned button,bool isPressed) { otherButtons[button].isPushed = isPressed; }

	inline bool GetIsExplicit()const { return isExplicit; }
	inline bool GetIsVisisble()const { return isVisible; }

	inline bool GetLeftMouseIsPressed()const { return leftMouseIsPressed; }
	inline bool GetMiddleMouseIsPressed()const { return middleMouseIsPressed; }
	inline bool GetRightMouseIsPressed()const { return rightMouseIsPressed; }

	inline bool GetOtherButtonIsPressed(unsigned button)const { return otherButtons[button].isPushed; }

	// Doesn't Include other Buttons
	inline bool GetAnyButtonIsPressed()const { return leftMouseIsPressed || rightMouseIsPressed || middleMouseIsPressed; }

	virtual void SetMousePos(Vec2I pos) = 0;
	virtual void SetMouseHidden(bool isHidden) = 0;
};
