#include  "stdafx.h"
#include "OpenGL2DRenderEngine.h"

#include <Utils/Statistics.h>

#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

#include <Core/2D/Components/Visible2DComponent.h>

#include <Utils/ListAddons.hpp>
#include <Utils/VectorAddons.hpp>

#include <Rendering/OpenGL/OpenGLFrameBuffer.h>

#include <Rendering/OpenGL/OpenGLTexture.h>
#include <Rendering/OpenGL/OpenGLContext.h>

#include <Rendering/OpenGL/Renderers/OpenGLLineRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLQuadRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLModelRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLSpriteRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLTriangleRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLLineLoopRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLQuadStripRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLTriangleStripRenderer.h>

OpenGL2DRenderEngine::OpenGL2DRenderEngine()
{
	renderLayers = new GL2DRenderLayers();
	context = 0;
}

OpenGL2DRenderEngine::~OpenGL2DRenderEngine()
{
	delete renderLayers;
	if (context)delete context;
}

void OpenGL2DRenderEngine::DrawScene(const Matrix44& cameraMatrix)
{

	DEBUG_BENCH_START_TRACK("OpenGLRenderEngine");

	for (auto layerIter : *renderLayers)
	{
		for (auto iter : *layerIter.second)
		{
			ShaderProgramBase* program = iter.first;
			program->BindProgram();

			for (VisibleComponentBase* component : iter.second)
			{
				component->PreRender();
				component->Render(cameraMatrix);
				component->PostRender();
			}
		}
	}

	DEBUG_TRACK_TAKE_BENCH("OpenGLRenderEngine");
}

void OpenGL2DRenderEngine::DrawDebugGUI()
{
	DEBUG_DRAW;

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OpenGL2DRenderEngine::AddComponent(Visible2DComponent* component)
{
	
	// TODO: sort into actual layers instead of this temp layer
	const unsigned layer = component->GetRenderLayer();
	GL2DRenderMap* map = 0;

	auto &layerIter = renderLayers->find(layer);
	if (layerIter != renderLayers->end())
	{
		map = layerIter->second;
		auto &iter = map->find(component->GetShaderProgram());
		if (iter != map->end())
		{
			iter->second.push_back(component);
		}
		else
		{
			map->insert({ component->GetShaderProgram(),{ component } });
		}
	}
	else
	{
		map = new GL2DRenderMap();
		map->insert({ component->GetShaderProgram(),{ component } });
		renderLayers->insert({ layer,map });
	}
}

bool OpenGL2DRenderEngine::RemoveComponent(Visible2DComponent* component)
{
	ShaderProgramBase* program = component->GetShaderProgram();

	for (auto outerTter : *renderLayers)
	{
		GL2DRenderMap* map = outerTter.second;
		if (map->find(program) != map->end())
		{
			std::vector<Visible2DComponent*>& objects = (*map)[program];
			auto& iter = std::find(objects.begin(), objects.end(), component);
			if (iter != objects.end())
			{
				objects.erase(iter);

				if (objects.size() == 0)
				{
					map->erase(program);
				}

				return true;
			}
		}
	}
	Log(LogLevel_Error, "OpenGLRenderEngine::RemoveRenderObject Failed to find and remove RenderObject!\n");

	return false;
}


void OpenGL2DRenderEngine::InitEngine(WindowHandle handle)
{
	context = new OpenGLContext(handle);
}


void OpenGL2DRenderEngine::ClearErrors()
{
	if(context)
		context->ClearErrors();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling ClearErrors  !! \n");
}

void OpenGL2DRenderEngine::EnableAlpha()
{
	if (context)
		context->EnableAlpha();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling EnableAlpha  !! \n");
}

void OpenGL2DRenderEngine::DisableAlpha()
{
	if (context)
		context->DisableAlpha();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling DisableAlpha  !! \n");
}

void OpenGL2DRenderEngine::ClearBuffers()
{
	if (context)
		context->ClearBuffers();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling ClearBuffers  !! \n");
}

void OpenGL2DRenderEngine::Resize(int w, int h)
{
	if (context)
		context->Resize(w, h);
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling Resize  !! \n");
}

void OpenGL2DRenderEngine::EnableDepthTesting()
{
	if (context)
		context->EnableDepthTesting();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling EnableDepthTesting  !! \n");
}

void OpenGL2DRenderEngine::DisableDepthTesting()
{
	if (context)
		context->DisableDepthTesting();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling DisableDepthTesting  !! \n");
}

void OpenGL2DRenderEngine::SetLineWidth(float width)
{
	if (context)
		context->SetLineWidth(width);
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling SetLineWidth  !! \n");
}

void OpenGL2DRenderEngine::CheckErrors(const char* text)
{
	if (context)
		context->CheckErrors(text);
	else
	{
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling CheckErrors  !! \n");
	}
}

TextureBase* OpenGL2DRenderEngine::CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat)
{
	if (context)
		return context->CreateTexture(path,bufferType,bufferFormat);
	else
	{
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling CreateTexture  !! \n");
		return 0;
	}
}

TextureBase* OpenGL2DRenderEngine::CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)
{
	if (context)
		return context->CreateTexture(data, bufferType, bufferFormat, size);
	else
	{
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling CreateTexture  !! \n");
		return 0;
	}
}

bool OpenGL2DRenderEngine::CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, Vec2I size, RenderDataType bufferType , RenderDataFormat bufferFormat )
{
	return context->CreateFrameBuffer(outBuffer, outTexture, bufferType, bufferFormat, size);
}

LineRenderer * OpenGL2DRenderEngine::CreateLineRenderer()
{
	//return new OpenGLLineRenderer(this);
	return 0;
}

LineLoopRenderer * OpenGL2DRenderEngine::CreateLineLoopRenderer()
{
	//return new OpenGLLineLoopRenderer(this);
	return 0;
}

ModelRenderer * OpenGL2DRenderEngine::CreateModelRenderer()
{
	//return OpenGLModelRenderer(this);
	return 0;
}

QuadRenderer * OpenGL2DRenderEngine::CreateQuadRenderer()
{
	//return new OpenGLQuadRenderer(this);
	return 0;
}

QuadStripRenderer * OpenGL2DRenderEngine::CreateQuadStripRenderer()
{
	//return new OpenGLQuadStripRenderer(this);
	return 0;
}

SpriteRenderer * OpenGL2DRenderEngine::CreateSpriteRenderer()
{
	return context->CreateSpriteRenderer();
}

TriangleRenderer * OpenGL2DRenderEngine::CreateTriangleRenderer()
{
	//return new OpenGLTriangleRenderer(this);
	return 0;
}

TriangleStripRenderer * OpenGL2DRenderEngine::CreateTriangleStripRenderer()
{
	return context->CreateTriangleStripRenderer();
}
