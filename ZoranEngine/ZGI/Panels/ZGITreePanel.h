#pragma once
#include <ZGI\Panels\ZGIPanel.h>
class ZGICollapsibleListPanel;
class ZoranEngine_EXPORT TreeSocket
{
private:
	// TODO: fix sizing issue and make first list not expandable
	ZGICollapsibleListPanel* list;
	std::vector<TreeSocket>* socketList;
	std::string* name; // name of Socket
	unsigned index; // index of list to parent list

	ZGIVirtualWindow* owningWindow;

public:
	TreeSocket(std::string name, unsigned index, ZGIVirtualWindow* owningWindow);
	TreeSocket(ZGIVirtualWindow* owningWindow);
	TreeSocket(TreeSocket& other);
	~TreeSocket();

	void AddWidget(ZGIWidget* widget);
	void AddText(std::string text, float fontSize);

	void SetText(std::string text);

	TreeSocket& TreeSocketNamed(std::string name);

	TreeSocket& operator=(TreeSocket&& other)
	{
		if (this != &other)
		{
			list = other.list;
			name = other.name;
			index = other.index;
			socketList = other.socketList;
			owningWindow = other.owningWindow;
		}
		return *this;
	}

	bool operator == (std::string name)
	{
		return *this->name == name;
	}

	bool operator == (unsigned index)
	{
		return this->index == index;
	}

	bool IsOpen()const;

	friend class ZGITreePanel;
};

class ZoranEngine_EXPORT ZGITreePanel : public ZGIPanel
{
private:
	TreeSocket rootSocket;

public:
	ZGITreePanel(ZGIVirtualWindow* owningWindow);
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

