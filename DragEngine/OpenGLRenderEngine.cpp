#include "stdafx.h"
#include "OpenGLRenderEngine.h"
#include "GL\glew.h"
#include "SceneObject.h"
#include "ShaderProgramBase.h"

#include <iostream>

OpenGLRenderEngine::OpenGLRenderEngine()
{
}

OpenGLRenderEngine::~OpenGLRenderEngine()
{
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
		PFD_DOUBLEBUFFER,               // Must Support Double Buffering
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
#endif
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

void OpenGLRenderEngine::DrawAll()
{
	for (auto iter : renderMap)
	{
		ShaderProgramBase* program = iter.first;
		for (SceneObject* object : iter.second)
		{
			object->PreRender();
			object->RenderScene();
			object->PostRender();
		}
	}
}

void OpenGLRenderEngine::Resize(int x, int y)
{
	glViewport(0, 0, x, y);
}

void OpenGLRenderEngine::AddSceneObject(SceneObject* object)
{
	ShaderProgramBase* program = object->GetShaderProgram();
	if (renderMap.find(program) != renderMap.end())
	{
		renderMap[program].push_back(object);
	}
	else
	{
		std::vector<SceneObject*> objects;
		objects.push_back(object);
		renderMap.insert(GLRenderMapPair(program, objects));
	}
}

bool OpenGLRenderEngine::RemoveSceneObject(SceneObject* object)
{
	ShaderProgramBase* program = object->GetShaderProgram();
	if (renderMap.find(program) != renderMap.end())
	{
		auto objects = renderMap[program];
		auto iter = std::find(objects.begin(), objects.end(), object);
		if (iter != objects.end())
		{
			objects.erase(iter);
			delete object;
			return true;
		}
	}

	std::cerr << "OpenGLRenderEngine::RemoveSceneObject Failed to find and remove SceneObject!\n";

	return false;
}

TextureBase * OpenGLRenderEngine::CreateTexture(const char * path, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size)
{
	return nullptr;
}

TextureBase * OpenGLRenderEngine::CreateTexture(void * data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size)
{
	return nullptr;
}

VertexBufferBase * OpenGLRenderEngine::CreateVertexBuffer(RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size, void * data)
{
	return nullptr;
}

bool OpenGLRenderEngine::CreateFrameBuffer(FrameBufferBase ** outBuffer, TextureBase ** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size)
{
	return false;
}

ShaderProgramBase * OpenGLRenderEngine::CreateShaderProgram(const char * vertex, const char * fragment)
{
	return nullptr;
}
