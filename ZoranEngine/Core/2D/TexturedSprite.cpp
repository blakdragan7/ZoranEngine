#include "stdafx.h"
#include "TexturedSprite.h"

#include <Core/ZoranEngine.h>
#include <Core/2D/Components/TexturedSpriteComponent.h>
#include <Physics/PhysicsEngine.h>
#include <Rendering/TextureBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/RenderedObjectBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Physics/2D/Collision/b2DCollision2DObject.h>
#include <Physics/2D/Collision/SatCollision2DObject.h>

TexturedSprite::TexturedSprite(unsigned renderLayer, std::string name) : SceneObject2D(name)
{
	spriteComponent = new TexturedSpriteComponent(renderLayer);
	spriteComponent->SetStartingSize(2, 2);
	spriteComponent->SetScale(1, 1);

	root2DComponent = spriteComponent;

	SetRootComponent(root2DComponent);

}

TexturedSprite::TexturedSprite(unsigned renderLayer, std::string name, const char* texture) : SceneObject2D(name)
{
	spriteComponent = new TexturedSpriteComponent(renderLayer, texture);
	spriteComponent->SetStartingSize(2, 2);
	spriteComponent->SetScale(1, 1);

	root2DComponent = spriteComponent;
	SetRootComponent(root2DComponent);
}

TexturedSprite::~TexturedSprite()
{
	if (rootComponent)delete rootComponent;
}

void TexturedSprite::SetTexture(const char* path)
{
	((TexturedSpriteComponent*)rootComponent)->SetTexture(path);
}

void TexturedSprite::SetTexture(ImageResource texture)
{
	((TexturedSpriteComponent*)rootComponent)->SetTexture(texture);
}
