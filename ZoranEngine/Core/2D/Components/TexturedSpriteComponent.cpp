#include "stdafx.h"
#include "TexturedSpriteComponent.h"
#include <Rendering/TextureBase.h>
#include <Rendering/ShaderProgramBase.h>
#include <Rendering/TextureManager.h>
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/RenderEngineBase.h>
#include <OpenGL/StandardShader2D.h>

TexturedSpriteComponent::TexturedSpriteComponent()
{
	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader2D>(StandardShader2D::initMap);

	rEngine->EnableAlpha();

	renderedObject->MakeFullScreenQuad();

	SetShaderProgram(program);
}

TexturedSpriteComponent::TexturedSpriteComponent(const char* texture, RenderDataType type, RenderDataFormat format)
{
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader2D>(StandardShader2D::initMap);

	renderedObject->MakeFullScreenQuad();

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

	GetShaderProgram()->SetModelMatrix(GetWorldMatrix());
}

void TexturedSpriteComponent::Render()
{
	Visible2DComponent::Render();
	renderedObject->RenderObject();
}

void TexturedSpriteComponent::PostRender()
{
	Visible2DComponent::PostRender();
}

void TexturedSpriteComponent::SetTexture(const char * texture, RenderDataType type, RenderDataFormat format)
{
	this->texture = TextureManager::GetInstance()->TextureForFilePath(texture, type, format);
}
