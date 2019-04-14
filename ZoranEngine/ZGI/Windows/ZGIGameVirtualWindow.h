#pragma once
#include <ZGI\Windows\ZGIVirtualWindow.h>

/*  
*	ZGIGameVirtualWindow is the window that the game view is rendered in. Specifically,					* 
*	the game view of the player. Therfore, there would be multiple instances of ZGIGameVirtualWindow	*
*	if there was more then one player locally, like in split scren.										*
*/

class ZGILabel;
class ZGIGameView;
class ZoranEngine_EXPORT ZGIGameVirtualWindow : public ZGIVirtualWindow
{
private:
	ZGIGameView* gameView;

public:
	ZGIGameVirtualWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, bool is3D=false, ZGIVirtualWindow* parent = 0);
	~ZGIGameVirtualWindow();

	virtual void RenderWindow(Vec2D globalOffset)override;
	virtual void OSWindowWasResized(Vec2I newSize)override;

	inline ZGIGameView* GetGameView()const { return gameView; }
};

