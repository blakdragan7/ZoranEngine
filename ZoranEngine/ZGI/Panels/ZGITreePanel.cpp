#include "stdafx.h"
#include "ZGITreePanel.h"
#include <ZGI/Panels/ZGICollapsibleListPanel.h>
#include <ZGI/Panels/ZGIGridPanel.h>
#include <ZGI/Widgets/ZGILabel.h>
#include <ZGI/Widgets/ZGIButton.h>

ZGITreePanel::ZGITreePanel(float treeSocketIndent, Vec2D treeSocketSize, ZGIVirtualWindow* owningWindow) : rootSocket(treeSocketIndent, treeSocketSize,"root", owningWindow), ZGIPanel(owningWindow)
{
}

ZGITreePanel::~ZGITreePanel()
{
}

bool ZGITreePanel::KeyEventSub(KeyEventType type, unsigned key)
{
	return rootSocket.KeyEventSub(type, key);
}

bool ZGITreePanel::ContainsWidget(ZGIWidget * widget) const
{
	return rootSocket.ContainsWidget(widget);
}

bool ZGITreePanel::CanAddWidget(ZGIWidget * widget) const
{
	return true;
}

int ZGITreePanel::GetNumberOfWidgets() const
{
	return rootSocket.GetNumberOfWidgets();
}

int ZGITreePanel::GetMaxNumberOfWidgets() const
{
	return INT_MAX;
}

ZGIWidget * ZGITreePanel::WidgetForPosition(Vec2D pos)
{
	return rootSocket.WidgetForPosition(pos);
}

void ZGITreePanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher Stuff
}

void ZGITreePanel::Render(const Matrix44 & projection)
{
	if (isDirty)
	{
	}

	rootSocket.Render(0);

	ZGIPanel::Render(projection);
}

void ZGITreePanel::Print(unsigned tabs) const
{
	ZGIWidget::Print(tabs);

	rootSocket.Print(tabs);
}

// TreeSocket

TreeSocket::TreeSocket(float indentSize, Vec2D size, std::string name, ZGIVirtualWindow * owningWindow) : indentSize(indentSize),socketSize(size), owningWindow(owningWindow)
{
	isCollapsible = true;
	isCollapsed = true;
	this->name = new std::string(name);
	socketList = new std::vector<TreeSocket>;
	panel = new ZGIGridPanel(10,1,owningWindow);
	headerButton = new ZGIButton(owningWindow);
}

TreeSocket::TreeSocket(TreeSocket & other)
{
	owningWindow = other.owningWindow;
	content = other.content;
	socketList = new std::vector<TreeSocket>(*other.socketList);
	name = new std::string(*other.name);
	socketSize = other.socketSize;
	indentSize = other.indentSize;
	isCollapsible = other.isCollapsible;
	isCollapsed = other.isCollapsed;
}

TreeSocket::~TreeSocket()
{
	delete name;
	delete socketList;
	delete content;
}

bool TreeSocket::KeyEventSub(KeyEventType type, unsigned key)
{
	if (content->KeyEvent(type, key))return true;
	for (auto& w : *socketList)
	{
		if (w.KeyEventSub(type,key))return true;
	}
	return false;
}

bool TreeSocket::ContainsWidget(ZGIWidget * widget) const
{
	if (content == widget)return true;

	for (auto& w : *socketList)
	{
		if (w.ContainsWidget(widget))return true;
	}

	return false;
}

int TreeSocket::GetNumberOfWidgets() const
{
	// we have the number of sockets we hold plus our own
	// this doesnt include the button widget or the gridpanel used for the header
	int number = socketList->size() + 1;

	for (auto& w : *socketList)
	{
		number += w.GetNumberOfWidgets();
	}

	return number;
}

ZGIWidget * TreeSocket::WidgetForPosition(Vec2D pos)
{
	if (auto w = content->HitTest(pos))return w;
	for (auto& s : *socketList)
	{
		if (auto w = s.WidgetForPosition(pos))return w;
	}
	return nullptr;
}

void TreeSocket::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	//TODO: ancher stuff
}

void TreeSocket::SetContent(ZGIWidget * widget)
{
	if (content)delete content;
	if (labelContent)labelContent = 0;
	content = widget;
	panel->AddWidget(widget, 1, 0, 9, 1);
}

void TreeSocket::SetText(std::string text)
{
	if (labelContent == 0)
	{
		labelContent = new ZGILabel(owningWindow);
		if (content)delete content;
		content = labelContent;
	}

	labelContent->SetText(text);
	panel->AddWidget(labelContent, 1, 0, 9, 1);
}

void TreeSocket::Print(int tabs)const
{
	content->Print(tabs);
}

void TreeSocket::Render(unsigned indentLevel)
{

}

TreeSocket & TreeSocket::TreeSocketNamed(std::string name)
{
	auto& itr = std::find(socketList->begin(), socketList->end(), name);
	if (itr != socketList->end())return *itr;

	socketList->push_back({indentSize,socketSize,name,owningWindow});
	
	TreeSocket& socket = (*socketList)[socketList->size() - 1];

	return socket;
}

bool TreeSocket::operator==(std::string name)
{
	return *this->name == name;
}
