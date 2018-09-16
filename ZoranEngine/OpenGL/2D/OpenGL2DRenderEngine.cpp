#include  "stdafx.h"
#include "OpenGL2DRenderEngine.h"

#include <Utils/Statistics.h>

#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

#include <Core/Components/VisibleComponentBase.h>
#include <Core/Components/ComponentBase.h>

#include <Utils/ListAddons.hpp>

OpenGL2DRenderEngine::OpenGL2DRenderEngine()
{
	renderMap = new GL2DRenderMap();
	renderLayers = new GL2DRenderLayers();
}

OpenGL2DRenderEngine::~OpenGL2DRenderEngine()
{
	delete renderMap;
}

void OpenGL2DRenderEngine::DrawAll()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	DEBUG_BENCH_START_TRACK("OpenGLLayerSorting");

	if (renderMap->size() > 0)
	{
		for (auto iter : *renderMap)
		{
			// TODO: sort into actual layers instead of this temp layer
			const unsigned layer = 0;
			GL2DRenderMap* map = 0;
			if (renderLayers->size() > layer)
			{
				map = (*renderLayers)[layer];
			}
			else
			{
				map = new GL2DRenderMap();
				renderLayers->insert(renderLayers->begin() + layer, map);
			}

			map->insert(iter);
		}

		renderMap->clear();
	}

	DEBUG_TRACK_TAKE_BENCH("OpenGLLayerSorting");

	DEBUG_BENCH_START_TRACK("OpenGLRenderEngine");

	for (auto layerIter : *renderLayers)
	{
		for (auto iter : *layerIter)
		{
			ShaderProgramBase* program = iter.first;
			program->BindProgram();

			for (VisibleComponentBase* component : iter.second)
			{
				component->PreRender();
				component->Render();
				component->PostRender();
			}
		}
	}

	DEBUG_TRACK_TAKE_BENCH("OpenGLRenderEngine");

	DEBUG_TAKE_BENCH;

	DEBUG_DRAW;

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void OpenGL2DRenderEngine::AddComponent(VisibleComponentBase* component)
{
 	ShaderProgramBase* program = component->GetShaderProgram();
	if (renderMap->find(program) != renderMap->end())
	{
		(*renderMap)[program].push_back(component);
	}
	else
	{
		std::vector<VisibleComponentBase*> objects;
		objects.push_back(component);
		renderMap->insert(GL2DRenderMapPair(program, objects));
	}
}

bool OpenGL2DRenderEngine::RemoveComponent(VisibleComponentBase* component)
{
	ShaderProgramBase* program = component->GetShaderProgram();

	if (renderMap->find(program) != renderMap->end())
	{
		std::vector<VisibleComponentBase*>& objects = (*renderMap)[program];
		auto& iter = std::find(objects.begin(), objects.end(), component);
		if (iter != objects.end())
		{
			objects.erase(iter);

			if (objects.size() == 0)
			{
				renderMap->erase(program);
			}

			return true;
		}
	}

	for (auto outerTter : *renderLayers)
	{
		if (outerTter->find(program) != outerTter->end())
		{
			std::vector<VisibleComponentBase*>& objects = (*outerTter)[program];
			auto& iter = std::find(objects.begin(), objects.end(), component);
			if (iter != objects.end())
			{
				objects.erase(iter);

				if (objects.size() == 0)
				{
					outerTter->erase(program);
				}

				return true;
			}
		}
	}
	Log(LogLevel_Error, "OpenGLRenderEngine::RemoveRenderObject Failed to find and remove RenderObject!\n");

	return false;
}