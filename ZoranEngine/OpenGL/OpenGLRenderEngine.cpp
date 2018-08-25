#include "stdafx.h"
#include "OpenGLRenderEngine.h"
#include "GL\glew.h"
#include "Core/SceneObject.h"
#include "Rendering/ShaderProgramBase.h"
#include "OpenGLObject.h"
#include "OpenGLTexture.h"
#include <Windows.h>
#include <iostream>

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
	//glEnable(GL_DEPTH_TEST);
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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	HighPrecisionClock clock;

	long long bindProgram,SetupShader,PreRender,Render,PostRender;
	bindProgram = SetupShader = PreRender = Render = PostRender = 0;

	DEBUG_BENCH_START_TRACK("OpenGLRenderEngine")
	for (auto iter : renderMap)
	{
		ShaderProgramBase* program = iter.first;
		clock.TakeClock();
		program->BindProgram();
		bindProgram += clock.GetDiffNanoSeconds();
		for (SceneObject* object : iter.second)
		{
			clock.TakeClock();
			program->SetupShaderFromSceneObject(object);
			SetupShader += clock.GetDiffNanoSeconds();

			clock.TakeClock();
			object->PreRender();
			PreRender += clock.GetDiffNanoSeconds();
			clock.TakeClock();
			object->RenderScene();
			Render += clock.GetDiffNanoSeconds();
			clock.TakeClock();
			object->PostRender();
			PostRender += clock.GetDiffNanoSeconds();
		}
	}

	DEBUG_ADD_STAT(bindProgram,"OpenGLRenderEngine", "BindProgram");
	DEBUG_ADD_STAT(SetupShader,"OpenGLRenderEngine", "SetupShader");
	DEBUG_ADD_STAT(PreRender,"OpenGLRenderEngine", "PreRender");
	DEBUG_ADD_STAT(Render,"OpenGLRenderEngine", "RenderScene");
	DEBUG_ADD_STAT(PostRender,"OpenGLRenderEngine", "PostRender");

	DEBUG_TRACK_TAKE_BENCH("OpenGLRenderEngine");

	DEBUG_TAKE_BENCH

	DEBUG_DRAW;

	//pEngine->GetCollisionBucketRootFor2D()->ImGuiDraw();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
		std::vector<SceneObject*>& objects = renderMap[program];
		auto& iter = std::find(objects.begin(), objects.end(), object);
		if (iter != objects.end())
		{
			objects.erase(iter);

			if (objects.size() == 0)
			{
				renderMap.erase(program);
			}

			return true;
		}
	}

	std::cerr << "OpenGLRenderEngine::RemoveSceneObject Failed to find and remove SceneObject!\n";

	return false;
}

void OpenGLRenderEngine::SetupScene(Vector3D center, Vector3D size)
{

}

TextureBase * OpenGLRenderEngine::CreateTexture(const char * path, RenderDataType bufferType, RenderDataFormat bufferFormat)
{
	OpenGLTexture* texture = new OpenGLTexture(this,bufferType,bufferFormat);
	texture->LoadFromPath(path);
	return texture;
}

TextureBase * OpenGLRenderEngine::CreateTexture(void * data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)
{
	return nullptr;
}

RenderedObjectBase * OpenGLRenderEngine::CreateRenderedObject()
{
	OpenGLObject* object = new OpenGLObject();
	return object;
}

bool OpenGLRenderEngine::CreateFrameBuffer(FrameBufferBase ** outBuffer, TextureBase ** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)
{
	return false;
}

ShaderProgramBase * OpenGLRenderEngine::CreateShaderProgram(const char * vertex, const char * fragment)
{
	return nullptr;
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
			std::cerr << "Error " << text << ": glError " << errorS << std::endl;
		}
}

void OpenGLRenderEngine::ClearErrors()
{
	int error;
	while ((error = glGetError()) != GL_NO_ERROR) {
	}
}
