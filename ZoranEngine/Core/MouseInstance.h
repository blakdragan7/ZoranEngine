#pragma once
#include <Math/Vector2.h>

/* Mouse Flags */

/*  
	Mouse Instance 

	Mouse Instance is used to track the mouse information on the window
*/

class MouseInstance
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
	MouseInstance(Vec2D pos) : position(pos) {}
	~MouseInstance() {}

	inline void SetWindowSpacePosition(Vec2D pos) { deltaPosition = (pos - position); position = pos; }

	inline Vec2D WindowSpacePosition()const { return position; }
	inline Vec2D GetWindowSpaceDelta()const { return deltaPosition; }

	inline void SetIsExplicit(bool isExplicit) { this->isExplicit = isExplicit; }
	inline void SetIsVisisble(bool isVisible) { this->isVisible = isVisible; }

	inline void SetLeftMouseIsPressed(bool isPressed) { leftMouseIsPressed = isPressed; }
	inline void SetMiddleMouseIsPressed(bool isPressed) { middleMouseIsPressed = isPressed; }
	inline void SetRightMouseIsPressed(bool isPressed) { rightMouseIsPressed = isPressed; }

	inline void SetOtherButtonIsPressed(unsigned button,bool isPressed) { otherButtons[button].isPushed = isPressed; }

	inline bool GetIsExplicit() { return isExplicit; }
	inline bool GetIsVisisble() { return isVisible; }

	inline bool GetLeftMouseIsPressed() { return leftMouseIsPressed; }
	inline bool GetMiddleMouseIsPressed() { return middleMouseIsPressed; }
	inline bool GetRightMouseIsPressed() { return rightMouseIsPressed; }

	inline bool GetOtherButtonIsPressed(unsigned button) { return otherButtons[button].isPushed; }
};
