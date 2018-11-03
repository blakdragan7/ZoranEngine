#include "stdafx.h"
#include "OpenGLContext.h"
#include "OpenGLTexture.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "OpenGLShaderProgramBase.h"

#include <GL\glew.h>
#include <Core/SceneObject.h>

#include "OpenGLFrameBuffer.h"

#include "Renderers/OpenGLLineRenderer.h"
#include "Renderers/OpenGLQuadRenderer.h"
#include "Renderers/OpenGLModelRenderer.h"
#include "Renderers/OpenGLSpriteRenderer.h"
#include "Renderers/OpenGLTriangleRenderer.h"
#include "Renderers/OpenGLLineLoopRenderer.h"
#include "Renderers/OpenGLQuadStripRenderer.h"
#include "Renderers/OpenGLTriangleStripRenderer.h"
#include "Renderers/OpenGLFontRenderer.h"
#include "Renderers/OpenGLGUIRenderer.h"

#include <Rendering/ShaderProgramBase.h>

#include <Physics/PhysicsEngine.h>
#include <Physics/2D/Collision/CollisionBucket2DBase.h>

#include <Utils/Statistics.h>

#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

OpenGLContext::OpenGLContext(WindowHandle handle)
{
#ifdef _WIN32
	HWND hwnd = static_cast<HWND>(handle);
	HDC dc = GetDC(hwnd);

	if (!dc)
	{
		std::cout << "Error Getting HDC of Window " << GetLastError() << "\n";
	}

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |            // Format Must Support Window
		PFD_SUPPORT_OPENGL |            // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,               // Must Support float Buffering
		PFD_TYPE_RGBA,                  // Request An RGBA Format
		24,                             // Select Our Color Depth
		0, 0, 0, 0, 0, 0,               // Color Bits Ignored
		0,								// No Alpha Buffer
		0,                              // Shift Bit Ignored
		0,                              // No Accumulation Buffer
		0, 0, 0, 0,                     // Accumulation Bits Ignored
		16,                             // 16Bit Z-Buffer (Depth Buffer)
		0,                              // No Stencil Buffer
		0,                              // No Auxiliary Buffer
		PFD_MAIN_PLANE,                 // Main Drawing Layer
		0,                              // Reserved
		0, 0, 0                         // Layer Masks Ignored
	};

	UpdateWindow(hwnd);

	int pixelFormat = ChoosePixelFormat(dc, &pfd);

	SetPixelFormat(dc, pixelFormat, &pfd);

	HGLRC context = wglCreateContext(dc);

	wglMakeCurrent(dc, context);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cerr << "Could not Init Glew ! " << res << std::endl;
		exit(0);
	}

	// Setup Dear ImGui binding

	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
#endif
}

OpenGLContext::~OpenGLContext()
{
	ImGui_ImplOpenGL3_Shutdown();


#ifdef _WIN32
	wglDeleteContext((HGLRC)context);
#endif
}

void OpenGLContext::EnableDepthTesting()
{
	glEnable(GL_DEPTH_TEST);
	CheckErrors("EnableDepthTesting");
}

void OpenGLContext::DisableDepthTesting()
{
	glDisable(GL_DEPTH_TEST);
	CheckErrors("DisableDepthTesting");
}

void OpenGLContext::DisableAlpha()
{
	glDisable(GL_BLEND);
	CheckErrors("DisableAlpha");
}

void OpenGLContext::EnableAlpha()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CheckErrors("EnableAlpha");
}

void OpenGLContext::ClearBuffers()
{
	glClearColor(1.0,0.0,0.0,1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CheckErrors("ClearBuffers");
}

void OpenGLContext::Resize(int x, int y,int width, int height)
{
	glViewport(x, y, width, height);
	CheckErrors("glViewport");
}

OpenGLTexture * OpenGLContext::CreateTexture(const char * path, RenderDataType bufferType, RenderDataFormat bufferFormat)
{
	OpenGLTexture* texture = new OpenGLTexture(this,bufferType,bufferFormat);
	texture->LoadFromPath(path);
	return texture;
}

OpenGLTexture * OpenGLContext::CreateTexture(void * data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)
{
	OpenGLTexture* texture = new OpenGLTexture(this,bufferType,bufferFormat);
	texture->LoadFromMemory(size.x, size.y, data, bufferType, bufferFormat);
	return texture;
}

bool OpenGLContext::CreateFrameBuffer(FrameBufferBase ** outBuffer, TextureBase ** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)
{
	OpenGLFrameBuffer* frameBuffer = new OpenGLFrameBuffer(size, bufferType, bufferFormat, this);
	*outBuffer = frameBuffer;
	*outTexture = frameBuffer->GetTargetTexture();

	return true;
}

void OpenGLContext::SetLineWidth(float width)
{
	glLineWidth(width);
	CheckErrors("EnableDepthTesting");
}

bool OpenGLContext::CheckErrors(const char* text)
{
		int error;
		bool hasError = false;
		while ((error = glGetError()) != GL_NO_ERROR) {
			const GLubyte* errorS = gluErrorString(error);
			if (!errorS)
				errorS = (const GLubyte*)" ";
			Log(LogLevel_Error,"%s GL Error %s",text,errorS);
			hasError = true;
		}

		return hasError;
}

void OpenGLContext::ClearErrors()
{
	int error;
	while ((error = glGetError()) != GL_NO_ERROR) {
	}
}

OpenGLLineRenderer * OpenGLContext::CreateLineRenderer()
{
	//return new OpenGLLineRenderer(this);
	return 0;
}

OpenGLLineLoopRenderer * OpenGLContext::CreateLineLoopRenderer()
{
	return new OpenGLLineLoopRenderer(this);
}

OpenGLModelRenderer * OpenGLContext::CreateModelRenderer()
{
	return new OpenGLModelRenderer(this);
}

OpenGLQuadRenderer * OpenGLContext::CreateQuadRenderer()
{
	//return new OpenGLQuadRenderer(this);
	return 0;
}

OpenGLQuadStripRenderer * OpenGLContext::CreateQuadStripRenderer()
{
	//return new OpenGLQuadStripRenderer(this);
	return 0;
}

OpenGLSpriteRenderer * OpenGLContext::CreateSpriteRenderer()
{
	return new OpenGLSpriteRenderer(this);
}

OpenGLTriangleRenderer * OpenGLContext::CreateTriangleRenderer()
{
	//return new OpenGLTriangleRenderer(this);
	return 0;
}

OpenGLTriangleStripRenderer * OpenGLContext::CreateTriangleStripRenderer()
{
	return new OpenGLTriangleStripRenderer(this);
}

OpenGLFontRenderer * OpenGLContext::CreateFontRenderer(FontResource* font)
{
	return new OpenGLFontRenderer(font, this);
}

OpenGLGUIRenderer * OpenGLContext::CreateGUIRenderer()
{
	return new OpenGLGUIRenderer(this);
}
