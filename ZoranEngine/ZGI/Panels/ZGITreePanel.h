#pragma once
#include <ZGI\Panels\ZGIPanel.h>
class ZGILabel;
class ZGIButton;
class ZGIGridPanel;
class TextureBase;
class ZoranEngine_EXPORT TreeSocket
{
private:
	bool wasMoved;

	ZGIGridPanel * panel;
	ZGIButton* headerButton;

	ZGILabel* labelContent; // content pre casted to label
	ZGIWidget* content;
	
	std::vector<TreeSocket>* socketList;
	std::string* name; // name of Socket
	
	bool isCollapsible;
	bool isCollapsed;
	
	bool needsPositioning;

	Vector2D socketSize;

	// during render the position is modified by indexSize * indentLevel for the x position
	float indentSize;
	int indentPosition;

	ZGIVirtualWindow* owningWindow;
	TreeSocket* parent;

	TextureBase* collapsedImage;
	TextureBase* openImage;
	TextureBase* emptyImage;

private:
	void SetParentModified();
	void SetImidiateChildrenParent(TreeSocket* parent);

public:
	TreeSocket(bool isCollapsible, int indentPosition, float indentSize, std::string name, TreeSocket* parent, ZGIVirtualWindow* owningWindow);
	TreeSocket(bool isCollapsible, int indentPosition, float indentSize, Vec2D size, std::string name, TreeSocket* parent, ZGIVirtualWindow* owningWindow);
	TreeSocket(TreeSocket& other) = delete;
	TreeSocket(TreeSocket&& other);
	~TreeSocket();

	void SetSize(Vec2D size);
	inline Vec2D GetSize() { return socketSize; }

	int GetNumberOfWidgets()const;
	bool ContainsWidget(ZGIWidget* widget)const;
	ZGIWidget* WidgetForPosition(Vec2D pos);
	void ContainerResized(Vec2D newSize, Vec2D oldSize);

	void SetContent(ZGIWidget* widget);
	void SetText(std::string text);

	void Print(int tabs)const;
	
	Vector2D UpdatePositionAndSize(Vector2D parentSize, Vector2D ParentPosition);

	void Render(const Matrix44& projection);
	TreeSocket& TreeSocketNamed(std::string name);

	void SetIsCollapsed(bool collapsed);

	bool operator == (std::string name)const;
	bool IsOpen()const { return isCollapsed == false || isCollapsible == false; }

	friend class ZGITreePanel;
};

// TODO: figure out scaling for resizing and such

class ZoranEngine_EXPORT ZGITreePanel : public ZGIPanel
{
private:
	TreeSocket rootSocket;
	float treeSocketHeight;

public:
	ZGITreePanel(float treeSocketIndent,ZGIVirtualWindow* owningWindow);
	~ZGITreePanel();

	void SetSocketSize(float size);

	inline TreeSocket& GetRootSocket() { return rootSocket; }

	/* Panel Override */

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

