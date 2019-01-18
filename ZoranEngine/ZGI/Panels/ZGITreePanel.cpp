#include "stdafx.h"
#include "ZGITreePanel.h"
#include <ZGI/Panels/ZGICollapsibleListPanel.h>
#include <ZGI/Panels/ZGIGridPanel.h>
#include <ZGI/Panels/ZGIUniformScalePanel.h>
#include <ZGI/Widgets/ZGILabel.h>
#include <ZGI/Widgets/ZGIButton.h>
#include <ZGI/Core/ZGIBrush.h>

#include <ZGI/Windows/ZGIVirtualWindow.h>

#include <Rendering/TextureManager.h>

ZGITreePanel::ZGITreePanel(float treeSocketIndent, ZGIVirtualWindow* owningWindow) : 
	treeSocketHeight(30), rootSocket(false, 0, treeSocketIndent, "root", 0, owningWindow), ZGIPanel(owningWindow)
{
	rootSocket.SetOwningTree(this);
}

ZGITreePanel::~ZGITreePanel()
{
}

void ZGITreePanel::SetSocketSize(float tsize)
{
	treeSocketHeight = tsize;
	rootSocket.SetSize({ size.w,tsize });
}

void ZGITreePanel::RemoveWidget(ZGIWidget * widget)
{
}

void ZGITreePanel::AnimateAllWidgets(float dt)
{
	rootSocket.Animate(dt);
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
			rootSocket.SetSize({size.x,treeSocketHeight});
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

void TreeSocket::SetImidiateChildrenParent(TreeSocket * parent)
{
	for (auto &s : *socketList)
	{
		s.parent = parent;
	}
}

TreeSocket::TreeSocket(bool isCollapsible, int indentPosition, float indentSize, std::string name, TreeSocket * parent, ZGIVirtualWindow * owningWindow) : needsPositioning(false),
isCollapsed(true), isCollapsible(isCollapsible), indentPosition(indentPosition), labelContent(0), content(0), indentSize(indentSize), parent(parent), owningWindow(owningWindow), wasMoved(false)
{
	if (parent)
		owningTree = parent->owningTree;

	this->name = new std::string(name);
	socketList = new std::vector<TreeSocket>;
	panel = owningWindow->SpawnWidget<ZGIGridPanel>(10, 1);

	headerButton = owningWindow->SpawnWidget<ZGIButton>();
	headerButton->SetButtonReleasedFunction([this](ZGIButton*)
	{this->SetIsCollapsed(!this->isCollapsed); });
	collapsedImage = tManager->TextureForFilePath("right-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	openImage = tManager->TextureForFilePath("down-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	emptyImage = tManager->TextureForFilePath("black_circle.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	headerButton->GetBrush()->SetBackgroudImage(emptyImage);

	if (isCollapsible)
	{
		ZGIUniformScalePanel* uniform = new ZGIUniformScalePanel(owningWindow);
		uniform->AddWidget(headerButton);
		panel->AddWidget(uniform, 0, 0, 1, 1);
	}
}

TreeSocket::TreeSocket(bool isCollapsible, int indentPosition, float indentSize, Vec2D size, std::string name, TreeSocket* parent, ZGIVirtualWindow * owningWindow) : isCollapsible(isCollapsible), needsPositioning(false),
isCollapsed(true), indentPosition(indentPosition), labelContent(0), content(0), indentSize(indentSize),socketSize(size), parent(parent), owningWindow(owningWindow), wasMoved(false)
{
	if (parent)
		owningTree = parent->owningTree;

	this->name = new std::string(name);
	socketList = new std::vector<TreeSocket>;
	panel = owningWindow->SpawnWidget<ZGIGridPanel>(10,1);

	headerButton = owningWindow->SpawnWidget<ZGIButton>();
	headerButton->SetButtonReleasedFunction([this](ZGIButton*)
	{this->SetIsCollapsed(!this->isCollapsed); });
	collapsedImage = tManager->TextureForFilePath("right-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	openImage = tManager->TextureForFilePath("down-arrow.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	emptyImage = tManager->TextureForFilePath("black_circle.png", Render_Data_Type_RGBA_32, Render_Data_Format_Float);
	headerButton->GetBrush()->SetBackgroudImage(emptyImage);

	if (isCollapsible)
	{
		ZGIUniformScalePanel* uniform = new ZGIUniformScalePanel(owningWindow);
		uniform->AddWidget(headerButton);
		panel->AddWidget(uniform, 0, 0, 1, 1);
	}

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
	other.SetImidiateChildrenParent(this);
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
	headerButton->SetButtonReleasedFunction([this](ZGIButton*)
	{this->SetIsCollapsed(!this->isCollapsed); });
	other.wasMoved = true;
}

TreeSocket::~TreeSocket()
{
	if (wasMoved)return;
	delete name;
	delete socketList;

	// TODO: uncomment this after reference counting implemented
	//tManager->DestroyTexture(collapsedImage);
	//tManager->DestroyTexture(openImage);
	//tManager->DestroyTexture(emptyImage);
}

void TreeSocket::SetSize(Vec2D size)
{
	socketSize = size;

	for (auto& s : *socketList)
	{
		s.SetSize(size);
	}
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
	if (auto w = panel->WidgetForPosition(pos))return w;
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
	if (widget == content)return;
	if (labelContent)labelContent = 0;
	content = widget;
	if (isCollapsible)
		panel->AddWidget(widget, 1, 0, 9, 1);
	else
		panel->AddWidget(labelContent, 0, 0, 10, 1);
	SetParentModified();
}

void TreeSocket::SetText(std::string text)
{
	if (labelContent == 0)
	{
		labelContent = new ZGILabel(owningWindow);
		if (content)delete content;
		content = labelContent;
		if (isCollapsible)
			panel->AddWidget(labelContent, 1, 0, 9, 1);
		else
			panel->AddWidget(labelContent, 0, 0, 10, 1);
		SetParentModified();
	}

	labelContent->SetText(text);
}

bool TreeSocket::RemoveWidget(ZGIWidget * widget)
{
	if (content == widget)
	{
		widget->SetParent(0);
		return true;
	}
	
	for (auto& s : *socketList)
	{
		if(s.RemoveWidget(widget))return true;
	}

	return false;
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
	nextPosition.y -= socketSize.h;
	
	if (isCollapsed == false || isCollapsible == false)
	{
		for (size_t i = 0; i < socketList->size(); i++)
		{
			TreeSocket& s = (*socketList)[i];

			nextPosition = s.UpdatePositionAndSize(newSize, nextPosition);
		}
	}
	return nextPosition;
}

void TreeSocket::Render(const Matrix44& projection)
{
	panel->Render(projection);
	// if were are not collasped render all children
	if (isCollapsed == false || isCollapsible == false)
	{
		for (auto& s : *socketList)
		{
			s.Render(projection);
		}
	}
}

void TreeSocket::Animate(float dt)
{
	panel->Animate(dt);

	for (auto& s : *socketList)
	{
		s.Animate(dt);
	}
}

TreeSocket & TreeSocket::TreeSocketNamed(std::string name)
{
	auto& itr = std::find(socketList->begin(), socketList->end(), name);
	if (itr != socketList->end())return *itr;

	headerButton->GetBrush()->SetBackgroudImage(collapsedImage);

	socketList->push_back({ true, indentPosition + 1, indentSize,socketSize,name,this,owningWindow });
	
	TreeSocket& socket = (*socketList)[socketList->size() - 1];

	SetParentModified();

	return socket;
}

void TreeSocket::SetIsCollapsed(bool collapsed)
{
	if (socketList->size() == 0)return;

	if (collapsed)
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
