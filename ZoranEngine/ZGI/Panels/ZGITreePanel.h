#pragma once
#include <ZGI\Panels\ZGIPanel.h>
class ZGILabel;
class ZGIButton;
class ZGIGridPanel;
class ZoranEngine_EXPORT TreeSocket
{
private:
	ZGIGridPanel * panel;
	ZGIButton* headerButton;

	ZGILabel* labelContent; // content pre casted to label
	ZGIWidget* content;
	
	std::vector<TreeSocket>* socketList;
	std::string* name; // name of Socket
	
	bool isCollapsible;
	bool isCollapsed;
	
	Vector2D socketSize;

	// during render the position is modified by indexSize * indentLevel for the x position
	float indentSize;

	ZGIVirtualWindow* owningWindow;

public:
	TreeSocket(float indentSize, Vec2D size, std::string name, ZGIVirtualWindow* owningWindow);
	TreeSocket(TreeSocket& other);
	~TreeSocket();

	int GetNumberOfWidgets()const;
	bool KeyEventSub(KeyEventType type, unsigned key);
	bool ContainsWidget(ZGIWidget* widget)const;
	ZGIWidget* WidgetForPosition(Vec2D pos);
	void ContainerResized(Vec2D newSize, Vec2D oldSize);

	void SetContent(ZGIWidget* widget);
	void SetText(std::string text);

	void Print(int tabs)const;
	
	void Render(unsigned indentLevel);
	TreeSocket& TreeSocketNamed(std::string name);

	bool operator == (std::string name);
	bool IsOpen()const { return isCollapsed == false || isCollapsible == false; }

	friend class ZGITreePanel;
};

// TODO: figure out scaling for resizing and such

class ZoranEngine_EXPORT ZGITreePanel : public ZGIPanel
{
private:
	TreeSocket rootSocket;

public:
	ZGITreePanel(float treeSocketIndent, Vec2D treeSocketSize, ZGIVirtualWindow* owningWindow);
	~ZGITreePanel();

	inline TreeSocket& GetRootSocket() { return rootSocket; }

	/* Panel Override */

	virtual bool KeyEventSub(KeyEventType type, unsigned key)override;

	virtual bool ContainsWidget(ZGIWidget* widget)const override;
	virtual bool CanAddWidget(ZGIWidget* widget)const override;

	virtual int GetNumberOfWidgets()const override;
	virtual int GetMaxNumberOfWidgets()const override;

	virtual ZGIWidget* WidgetForPosition(Vec2D pos)override;

	/* Widget Override */

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual void Render(const Matrix44& projection)override;
	virtual const char* GetClassString()const override { return "ZGITreePanel"; }
	virtual void Print(unsigned tabs)const override;
};

