#include "stdafx.h"
#include "ZGITreePanel.h"
#include <ZGI/Panels/ZGICollapsibleListPanel.h>
#include <ZGI/Widgets/ZGILabel.h>

ZGITreePanel::ZGITreePanel(ZGIVirtualWindow* owningWindow) : rootSocket("root", -1, owningWindow), ZGIPanel(owningWindow)
{
}

ZGITreePanel::~ZGITreePanel()
{
	delete rootSocket.list;
}

bool ZGITreePanel::KeyEventSub(KeyEventType type, unsigned key)
{
	return rootSocket.list->KeyEventSub(type, key);
}

bool ZGITreePanel::ContainsWidget(ZGIWidget * widget) const
{
	return rootSocket.list->ContainsWidget(widget);
}

bool ZGITreePanel::CanAddWidget(ZGIWidget * widget) const
{
	return rootSocket.list->CanAddWidget(widget);
}

int ZGITreePanel::GetNumberOfWidgets() const
{
	return rootSocket.list->GetNumberOfWidgets();
}

int ZGITreePanel::GetMaxNumberOfWidgets() const
{
	return rootSocket.list->GetMaxNumberOfWidgets();
}

ZGIWidget * ZGITreePanel::WidgetForPosition(Vec2D pos)
{
	return rootSocket.list->WidgetForPosition(pos);
}

void ZGITreePanel::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	rootSocket.list->ContainerResized(newSize, oldSize);
}

void ZGITreePanel::Render(const Matrix44 & projection)
{
	rootSocket.list->Render(projection);
}

// TreeSocket

TreeSocket::TreeSocket(std::string name, unsigned index, ZGIVirtualWindow * owningWindow) : index(index), name(new std::string(name))
{
	list = new ZGICollapsibleListPanel(owningWindow);
}

TreeSocket::TreeSocket(ZGIVirtualWindow* owningWindow) : owningWindow(owningWindow), list(0) , index(-1)
{
	name = new std::string;
	socketList = new std::vector<TreeSocket>;
	list = new ZGICollapsibleListPanel(owningWindow);
}

TreeSocket::~TreeSocket()
{
	delete name;
	delete socketList;

	// dont delete list here because lists deconstructor will delete other lists including ones that might be held here
}

void TreeSocket::AddWidget(ZGIWidget * widget)
{
	list->AddWidget(widget);
}

void TreeSocket::AddText(std::string text, float fontSize)
{
	ZGILabel* label = new ZGILabel(owningWindow);
	label->SetText(text);
	label->SetFontSize(fontSize);
	AddWidget(label);
}

TreeSocket & TreeSocket::TreeSocketNamed(std::string name)
{
	// TODO: insert return statement here
	auto& itr = std::find(socketList->begin(), socketList->end(), name);
	if (itr != socketList->end())return *itr;

	unsigned index = list->GetNumberOfWidgets();

	TreeSocket socket(name,index,owningWindow);
	socketList->push_back(socket);
	list->AddWidget(socket.list);

	return *socketList->_Mylast();
}

bool TreeSocket::IsOpen() const
{
	return list->GetIsCollapsed() == false;
}
