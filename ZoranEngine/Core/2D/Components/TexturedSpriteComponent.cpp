#include "stdafx.h"
#include <Resources/ManagedResource.h>
#include <Resources/ImageAsset.h>
#include <Resources/ResourceManager.h>

#include "TexturedSpriteComponent.h"
#include <Rendering/TextureBase.h>
#include <Rendering/ShaderProgramBase.h>
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/OpenGL/2D/StandardShader2D.h>
#include <Rendering/Renderers/SpriteRenderer.h>


TexturedSpriteComponent::TexturedSpriteComponent(unsigned renderLayer) : Visible2DComponent(renderLayer)
{
	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader2D>();

	rEngine->EnableAlpha();

	renderedObject = rEngine->CreateSpriteRenderer();

	SetShaderProgram(program);
}

TexturedSpriteComponent::TexturedSpriteComponent(unsigned renderLayer, const char* texture)
	: Visible2DComponent(renderLayer)
{
	this->texture = RM->ImageForPath(texture);

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader2D>();

	renderedObject = rEngine->CreateSpriteRenderer();

	SetShaderProgram(program);
}

TexturedSpriteComponent::TexturedSpriteComponent(unsigned renderLayer, ImageResource texture) : texture(texture),
	Visible2DComponent(renderLayer)
{
	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader2D>();

	renderedObject = rEngine->CreateSpriteRenderer();

	SetShaderProgram(program);
}

TexturedSpriteComponent::~TexturedSpriteComponent()
{
}

void TexturedSpriteComponent::PreRender()
{
	Visible2DComponent::PreRender();

	if (texture.IsValid())texture->GetRenderAsset()->UseTexture(0);
}

void TexturedSpriteComponent::SetTexture(const char * resourcePath)
{
	this->texture = RM->ImageForPath(resourcePath);
}

void TexturedSpriteComponent::SetTexture(ImageResource texture)
{
	this->texture = texture;
}
