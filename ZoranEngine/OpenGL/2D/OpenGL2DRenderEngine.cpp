#include  "stdafx.h"
#include "OpenGL2DRenderEngine.h"

#include <Utils/Statistics.h>

#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

#include <Core/Components/VisibleComponentBase.h>
#include <Core/Components/ComponentBase.h>

#include <Utils/ListAddons.hpp>
#include <Utils/VectorAddons.hpp>

OpenGL2DRenderEngine::OpenGL2DRenderEngine()
{
	renderLayers = new GL2DRenderLayers();
}

OpenGL2DRenderEngine::~OpenGL2DRenderEngine()
{
	delete renderLayers;
}

void OpenGL2DRenderEngine::DrawAll()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

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

bool OpenGL2DRenderEngine::RemoveComponent(VisibleComponentBase* component)
{
	ShaderProgramBase* program = component->GetShaderProgram();

	for (auto outerTter : *renderLayers)
	{
		GL2DRenderMap* map = outerTter.second;
		if (map->find(program) != map->end())
		{
			std::vector<VisibleComponentBase*>& objects = (*map)[program];
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