#include "stdafx.h"
#include "OpenGLRenderEngine.h"
#include "OpenGLRenderObject.h"
#include "OpenGLTexture.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "OpenGLShaderProgramBase.h"

#include <GL\glew.h>


#include <Core/SceneObject.h>

#include <Rendering/ShaderProgramBase.h>

#include <Physics/PhysicsEngine.h>
#include <Physics/2D/Collision/CollisionBucket2DBase.h>

#include <Utils/Statistics.h>

#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

OpenGLRenderEngine::OpenGLRenderEngine()
{
	context = 0;
}

OpenGLRenderEngine::~OpenGLRenderEngine()
{
	ImGui_ImplOpenGL3_Shutdown();


#ifdef _WIN32
	wglDeleteContext((HGLRC)context);
#endif
}

void OpenGLRenderEngine::InitEngine(WindowHandle handle)
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

void OpenGLRenderEngine::EnableDepthTesting()
{
	glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderEngine::DisableDepthTesting()
{
	glDisable(GL_DEPTH_TEST);
}

void OpenGLRenderEngine::DisableAlpha()
{
	glDisable(GL_BLEND);
}

void OpenGLRenderEngine::EnableAlpha()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderEngine::ClearBuffers()
{
	glClearColor(1.0,0.0,0.0,1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderEngine::Resize(int x, int y)
{
	glViewport(0, 0, x, y);
}

TextureBase * OpenGLRenderEngine::CreateTexture(const char * path, RenderDataType bufferType, RenderDataFormat bufferFormat)
{
	OpenGLTexture* texture = new OpenGLTexture(this,bufferType,bufferFormat);
	texture->LoadFromPath(path);
	return texture;
}

TextureBase * OpenGLRenderEngine::CreateTexture(void * data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)
{
	OpenGLTexture* texture = new OpenGLTexture(this,bufferType,bufferFormat);
	texture->LoadFromMemory(size.x, size.y, data, bufferType, bufferFormat);
	return texture;
}

RenderedObjectBase * OpenGLRenderEngine::CreateRenderedObject()
{
	OpenGLRenderObject* object = new OpenGLRenderObject(this);
	return object;
}

bool OpenGLRenderEngine::CreateFrameBuffer(FrameBufferBase ** outBuffer, TextureBase ** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)
{
	return false;
}

void OpenGLRenderEngine::SetLineWidth(float width)
{
	glLineWidth(width);
}

void OpenGLRenderEngine::CheckErrors(const char* text)
{
		int error;
		while ((error = glGetError()) != GL_NO_ERROR) {
			const GLubyte* errorS = gluErrorString(error);
			if (!errorS)
				errorS = (const GLubyte*)" ";
			Log(LogLevel_Error,"%s GL Error %s",text,errorS);
		}
}

void OpenGLRenderEngine::ClearErrors()
{
	int error;
	while ((error = glGetError()) != GL_NO_ERROR) {
	}
}
