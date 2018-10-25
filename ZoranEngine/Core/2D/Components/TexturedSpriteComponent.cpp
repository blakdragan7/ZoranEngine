#include "stdafx.h"
#include "TexturedSpriteComponent.h"
#include <Rendering/TextureBase.h>
#include <Rendering/ShaderProgramBase.h>
#include <Rendering/TextureManager.h>
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

TexturedSpriteComponent::TexturedSpriteComponent(unsigned renderLayer, const char* texture, RenderDataType type, RenderDataFormat format)
	: Visible2DComponent(renderLayer)
{
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader2D>();

	renderedObject = rEngine->CreateSpriteRenderer();

	SetShaderProgram(program);
}

TexturedSpriteComponent::~TexturedSpriteComponent()
{
	TextureManager::GetInstance()->DestroyTexture(texture);
}

void TexturedSpriteComponent::PreRender()
{
	Visible2DComponent::PreRender();

	if (texture)texture->UseTexture(0);
}

void TexturedSpriteComponent::SetTexture(const char * texture, RenderDataType type, RenderDataFormat format)
{
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);
}

void TexturedSpriteComponent::SetTexture(TextureBase * texture)
{
	this->texture = texture;
}
