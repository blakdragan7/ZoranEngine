#include "stdafx.h"
#include "OpenGL3DRenderEngine.h"

#include <Utils/Statistics.h>
#include <Utils/VectorAddons.hpp>

#include <Core/3D/Components/Visible3DComponent.h>

#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

#include <Rendering/OpenGL/OpenGLContext.h>
#include <Rendering/OpenGL/OpenGLTexture.h>

#include <Rendering/OpenGL/OpenGLFrameBuffer.h>

#include <Rendering/OpenGL/Renderers/OpenGLLineRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLQuadRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLModelRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLSpriteRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLTriangleRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLLineLoopRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLQuadStripRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLTriangleStripRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLFontRenderer.h>
#include <Rendering/OpenGL/Renderers/OpenGLGUIRenderer.h>

OpenGL3DRenderEngine::OpenGL3DRenderEngine()
{
	renderMap = new GL3DRenderMap();
}


OpenGL3DRenderEngine::~OpenGL3DRenderEngine()
{
	delete renderMap;
}

void OpenGL3DRenderEngine::SetViewport(int x, int y, int width, int height)
{
	context->Resize(x, y, width, height);
}

void OpenGL3DRenderEngine::DrawScene(const Matrix44& cameraMatrix)
{
	ClearBuffers();

	DEBUG_BENCH_START_TRACK("OpenGLRenderEngine");

	DEBUG_TRACK_TAKE_BENCH("OpenGLRenderEngine");


	for (auto& iter : *renderMap)
	{
		iter.first->BindProgram();

		for (auto& innerIter : iter.second)
		{
			innerIter->PreRender();
			innerIter->Render(cameraMatrix);
			innerIter->PostRender();
		}
	}


}

void OpenGL3DRenderEngine::DrawDebugGUI()
{
	DEBUG_DRAW;

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OpenGL3DRenderEngine::AddComponent(Visible3DComponent * component)
{
	ShaderProgramBase* program = component->GetShaderProgram();
	auto &iter = renderMap->find(program);
	if (iter != renderMap->end())
	{
		iter->second.push_back(component);
	}
	else
	{
		renderMap->insert({ program,{component} });
	}
}

bool OpenGL3DRenderEngine::RemoveComponent(Visible3DComponent * component)
{
	ShaderProgramBase* program = component->GetShaderProgram();
	auto &iter = renderMap->find(program);
	if (iter != renderMap->end())
	{
		if (remove(iter->second, component))
			return true;
	}

	return false;
}

void OpenGL3DRenderEngine::InitEngine(WindowHandle handle)
{
	context = new OpenGLContext(handle);

	context->EnableDepthTesting();
}

void OpenGL3DRenderEngine::ClearErrors()
{
	if (context)
		context->ClearErrors();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling ClearErrors  !! \n");
}

void OpenGL3DRenderEngine::EnableAlpha()
{
	if (context)
		context->EnableAlpha();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling EnableAlpha  !! \n");
}

void OpenGL3DRenderEngine::DisableAlpha()
{
	if (context)
		context->DisableAlpha();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling DisableAlpha  !! \n");
}

void OpenGL3DRenderEngine::ClearBuffers()
{
	if (context)
		context->ClearBuffers();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling ClearBuffers  !! \n");
}

void OpenGL3DRenderEngine::Resize(int w, int h)
{
	if (context)
		context->Resize(0, 0, w, h);
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling Resize  !! \n");
}

void OpenGL3DRenderEngine::EnableDepthTesting()
{
	if (context)
		context->EnableDepthTesting();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling EnableDepthTesting  !! \n");
}

void OpenGL3DRenderEngine::DisableDepthTesting()
{
	if (context)
		context->DisableDepthTesting();
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling DisableDepthTesting  !! \n");
}

void OpenGL3DRenderEngine::SetLineWidth(float width)
{
	if (context)
		context->SetLineWidth(width);
	else
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling SetLineWidth  !! \n");
}

void OpenGL3DRenderEngine::CheckErrors(const char* text)
{
	if (context)
		context->CheckErrors(text);
	else
	{
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling CheckErrors  !! \n");
	}
}

TextureBase* OpenGL3DRenderEngine::CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat)
{
	if (context)
		return context->CreateTexture(path, bufferType, bufferFormat);
	else
	{
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling CreateTexture  !! \n");
		return 0;
	}
}

TextureBase* OpenGL3DRenderEngine::CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)
{
	if (context)
		return context->CreateTexture(data, bufferType, bufferFormat, size);
	else
	{
		Log(LogLevel_Error, "OpenGL Context Not Created While Calling CreateTexture  !! \n");
		return 0;
	}
}

bool OpenGL3DRenderEngine::CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, Vec2I size, RenderDataType bufferType, RenderDataFormat bufferFormat)
{
	return context->CreateFrameBuffer(outBuffer, outTexture, bufferType, bufferFormat, size);
}

LineRenderer * OpenGL3DRenderEngine::CreateLineRenderer()
{
	//return new OpenGLLineRenderer(this);
	return 0;
}

LineLoopRenderer * OpenGL3DRenderEngine::CreateLineLoopRenderer()
{
	//return new OpenGLLineLoopRenderer(this);
	return 0;
}

ModelRenderer * OpenGL3DRenderEngine::CreateModelRenderer()
{
	return context->CreateModelRenderer();
}

QuadRenderer * OpenGL3DRenderEngine::CreateQuadRenderer()
{
	//return new OpenGLQuadRenderer(this);
	return 0;
}

QuadStripRenderer * OpenGL3DRenderEngine::CreateQuadStripRenderer()
{
	//return new OpenGLQuadStripRenderer(this);
	return 0;
}

SpriteRenderer * OpenGL3DRenderEngine::CreateSpriteRenderer()
{
	return context->CreateSpriteRenderer();
}

TriangleRenderer * OpenGL3DRenderEngine::CreateTriangleRenderer()
{
	//return new OpenGLTriangleRenderer(this);
	return 0;
}

TriangleStripRenderer * OpenGL3DRenderEngine::CreateTriangleStripRenderer()
{
	return context->CreateTriangleStripRenderer();
}

FontRenderer * OpenGL3DRenderEngine::CreateFontRenderer()
{
	return context->CreateFontRenderer();
}

GUIRenderer * OpenGL3DRenderEngine::CreateGUIRenderer()
{
	return context->CreateGUIRenderer();
}
