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
	if (isDirty)
	{
		rootSocket.list->SetSize(size);
		rootSocket.list->SetPosition(position);
	}
	rootSocket.list->Render(projection);

	ZGIPanel::Render(projection);
}

void ZGITreePanel::Print(unsigned tabs) const
{
	ZGIWidget::Print(tabs);

	rootSocket.list->Print(tabs+1);
}

// TreeSocket

TreeSocket::TreeSocket(std::string name, unsigned index, ZGIVirtualWindow * owningWindow) : owningWindow(owningWindow), index(index)
{
	this->name = new std::string(name);
	list = new ZGICollapsibleListPanel(true, owningWindow);
	socketList = new std::vector<TreeSocket>;
}

TreeSocket::TreeSocket(ZGIVirtualWindow* owningWindow) : owningWindow(owningWindow), index(-1)
{
	name = new std::string;
	socketList = new std::vector<TreeSocket>;
	list = new ZGICollapsibleListPanel(true, owningWindow);
}

TreeSocket::TreeSocket(TreeSocket & other)
{
	owningWindow = other.owningWindow;
	list = other.list;
	socketList = new std::vector<TreeSocket>(*other.socketList);
	name = new std::string(*other.name);
	index = other.index;
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

void TreeSocket::SetText(std::string text)
{
	list->SetHeaderText(text);
}

bool TreeSocket::IsOpen()const
{
	return list->GetIsCollapsed() == false;
}

TreeSocket & TreeSocket::TreeSocketNamed(std::string name)
{
	auto& itr = std::find(socketList->begin(), socketList->end(), name);
	if (itr != socketList->end())return *itr;

	unsigned index = list->GetNumberOfWidgets();

	socketList->push_back({name,index,owningWindow});
	
	TreeSocket& socket = (*socketList)[socketList->size() - 1];
	
	list->AddWidget(socket.list);
	socket.list->SetCollapsed(true);

	return socket;
}
