#include "stdafx.h"
#include "ZGITreePanel.h"
#include <ZGI/Panels/ZGICollapsibleListPanel.h>
#include <ZGI/Panels/ZGIGridPanel.h>
#include <ZGI/Widgets/ZGILabel.h>
#include <ZGI/Widgets/ZGIButton.h>
#include <ZGI/Core/ZGIBrush.h>

#include <Rendering/TextureManager.h>

ZGITreePanel::ZGITreePanel(float treeSocketIndent, ZGIVirtualWindow* owningWindow) : rootSocket(0, treeSocketIndent, "root", 0, owningWindow), ZGIPanel(owningWindow)
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
	if (isDirty || rootSocket.needsPositioning)
	{
		if(isDirty)
			rootSocket.SetSize({size.x,size.y * 0.05f}); // TODO: make this not hard coded
		// convert to upperleft to make it easier for positioning
		rootSocket.UpdatePositionAndSize(size, { position.x , position.y + size.h });
		rootSocket.needsPositioning = false;
	}

	rootSocket.Render(projection);

	ZGIPanel::Render(projection);
}

void ZGITreePanel::Print(unsigned tabs) const
{
	ZGIWidget::Print(tabs);

	rootSocket.Print(tabs);
}

// TreeSocket

void TreeSocket::SetParentModified()
{
	if (parent)parent->SetParentModified();
	else needsPositioning = true;
}

TreeSocket::TreeSocket(int indentPosition, float indentSize, std::string name, TreeSocket * parent, ZGIVirtualWindow * owningWindow) : isCollapsible(false), needsPositioning(false),
isCollapsed(true), indentPosition(indentPosition), labelContent(0), content(0), indentSize(indentSize), parent(parent), owningWindow(owningWindow), wasMoved(false)
{
	this->name = new std::string(name);
	socketList = new std::vector<TreeSocket>;
	panel = new ZGIGridPanel(10, 1, owningWindow);
	headerButton = new ZGIButton(owningWindow);
	headerButton->SetButtonReleasedFunction([this]() {SetIsCollapsed(!this->IsOpen()); });
	panel->AddWidget(headerButton, 0, 0, 1, 1);
	collapsedImage = tManager->TextureForFilePath("right-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	openImage = tManager->TextureForFilePath("down-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	headerButton->GetBrush()->SetBackgroudImage(collapsedImage);
}

TreeSocket::TreeSocket(int indentPosition, float indentSize, Vec2D size, std::string name, TreeSocket* parent, ZGIVirtualWindow * owningWindow) : isCollapsible(false), needsPositioning(false),
isCollapsed(true), indentPosition(indentPosition), labelContent(0), content(0), indentSize(indentSize),socketSize(size), parent(parent), owningWindow(owningWindow), wasMoved(false)
{
	this->name = new std::string(name);
	socketList = new std::vector<TreeSocket>;
	panel = new ZGIGridPanel(10,1,owningWindow);
	headerButton = new ZGIButton(owningWindow);
	headerButton->SetButtonReleasedFunction([this]() {SetIsCollapsed(!this->IsOpen()); });
	panel->AddWidget(headerButton, 0, 0, 1, 1);
	collapsedImage = tManager->TextureForFilePath("right-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	openImage = tManager->TextureForFilePath("down-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	headerButton->GetBrush()->SetBackgroudImage(collapsedImage);
}

/*TreeSocket::TreeSocket(TreeSocket & other)
{
	owningWindow = other.owningWindow;
	content = other.content;
	panel = other.panel;
	headerButton = other.headerButton;
	labelContent = other.labelContent;
	socketList = new std::vector<TreeSocket>(*other.socketList);
	name = new std::string(*other.name);
	socketSize = other.socketSize;
	indentSize = other.indentSize;
	isCollapsible = other.isCollapsible;
	isCollapsed = other.isCollapsed;
	indentSize = other.indentSize;
	indentPosition = other.indentPosition;
	needsPositioning = other.needsPositioning;
}*/

TreeSocket::TreeSocket(TreeSocket && other)
{
	owningWindow = other.owningWindow;
	content = other.content;
	panel = other.panel;
	headerButton = other.headerButton;
	labelContent = other.labelContent;
	socketList = other.socketList;
	name = other.name;
	socketSize = other.socketSize;
	indentSize = other.indentSize;
	isCollapsible = other.isCollapsible;
	isCollapsed = other.isCollapsed;
	indentPosition = other.indentPosition;
	needsPositioning = other.needsPositioning;
	collapsedImage = other.collapsedImage;
	openImage = other.openImage;
	parent = other.parent;
	other.wasMoved = true;
}

TreeSocket::~TreeSocket()
{
	if (wasMoved)return;
	delete name;
	delete socketList;
	delete content;

	// TODO: uncomment this after reference counting implemented
	//tManager->DestroyTexture(collapsedImage);
	//tManager->DestroyTexture(openImage);
}

void TreeSocket::SetSize(Vec2D size)
{
	socketSize = size;

	for (auto& s : *socketList)
	{
		s.SetSize(size);
	}
}

bool TreeSocket::KeyEventSub(KeyEventType type, unsigned key)
{
	if(content)
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
	int number = static_cast<int>(socketList->size());

	for (auto& w : *socketList)
	{
		number += w.GetNumberOfWidgets();
	}

	return number;
}

ZGIWidget * TreeSocket::WidgetForPosition(Vec2D pos)
{
	if(content)
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
		panel->AddWidget(labelContent, 1, 0, 9, 1);
	}

	labelContent->SetText(text);
}

void TreeSocket::Print(int tabs)const
{
	panel->Print(tabs);
}

Vector2D TreeSocket::UpdatePositionAndSize(Vector2D parentSize, Vector2D parentPosition)
{
	Vector2D newPosition = parentPosition;
	Vector2D newSize = socketSize;

	float indent = indentSize * static_cast<float>(indentPosition);
	newPosition.y -= socketSize.h;
	newPosition.x += indent;

	newSize.x -= indent;

	panel->SetSize(newSize);
	panel->SetPosition(newPosition);

	Vector2D nextPosition(parentPosition);
	if (isCollapsed == false)nextPosition.y -= socketSize.h;

	for (size_t i =0;i<socketList->size();i++)
	{
		TreeSocket& s = (*socketList)[i];

		nextPosition = s.UpdatePositionAndSize(newSize, nextPosition);
	}
	
	return nextPosition;
}

void TreeSocket::Render(const Matrix44& projection)
{
	panel->Render(projection);

	// if were are not collasped render all children
	if (isCollapsed == false)
	{
		for (auto& s : *socketList)
		{
			s.Render(projection);
		}
	}
}

TreeSocket & TreeSocket::TreeSocketNamed(std::string name)
{
	auto& itr = std::find(socketList->begin(), socketList->end(), name);
	if (itr != socketList->end())return *itr;

	socketList->push_back({ indentPosition + 1, indentSize,socketSize,name,this,owningWindow });
	
	TreeSocket& socket = (*socketList)[socketList->size() - 1];

	SetParentModified();

	return socket;
}

void TreeSocket::SetIsCollapsed(bool collapsed)
{
	if (isCollapsed)
	{
		headerButton->GetBrush()->SetBackgroudImage(collapsedImage);
	}
	else
	{
		headerButton->GetBrush()->SetBackgroudImage(openImage);
	}

	this->isCollapsed = collapsed;
	SetParentModified();
}

bool TreeSocket::operator==(std::string name)const
{
	return *this->name == name;
}
