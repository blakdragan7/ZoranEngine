#pragma once
#include <ZGI\Windows\ZGIVirtualWindow.h>

/*  
*	ZGIGameVirtualWindow is the window that the game view is rendered in. Specifically,					* 
*	the game view of the player. Therfore, there would be multiple instances of ZGIGameVirtualWindow	*
*	if there was more then one player locally, like in split scren.										*
*/

class TriangleStripRenderer;
class PlayerInstanceBase;
class ShaderProgramBase;
class ZGILabel;
class ZoranEngine_EXPORT ZGIGameVirtualWindow : public ZGIVirtualWindow
{
private:
	PlayerInstanceBase * player;
	TriangleStripRenderer* fullScreenRenderer;
	ShaderProgramBase* fullScreenProgram;
	ZGILabel* fpsLabel;

	bool is3DCache;

public:
	ZGIGameVirtualWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, bool is3D=false, ZGIVirtualWindow* parent = 0);
	~ZGIGameVirtualWindow();

	void SetFPS(double fps);

	virtual void RenderWindow(Vec2D globalOffset)override;
	virtual void OSWindowWasResized(Vec2I newSize)override;
	inline void SetIs3D(bool is3D) { is3DCache = is3D; }
	inline bool GetIs3D() { return is3DCache; }
	inline void SetPlayerInstance(PlayerInstanceBase* player) { this->player = player; }
	inline PlayerInstanceBase* GetPlayer() { return player; }
};

