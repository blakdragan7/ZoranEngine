#include "stdafx.h"
#include "ModelRenderer.h"

#include <ThirdParty/assimp/scene.h>
#include <ThirdParty/assimp/Importer.hpp>
#include <ThirdParty/assimp/Exporter.hpp>

ModelRenderer::ModelRenderer(PrimitiveType pt, DrawType dt) : scene(0), hasLoadedFile(false), RenderedObjectBase(pt, VT_Float, dt)
{
}

ModelRenderer::ModelRenderer(const char* file, PrimitiveType pt, DrawType dt) : scene(0), hasLoadedFile(false), RenderedObjectBase(pt, VT_Float, dt)
{
	LoadFile(file);
}

ModelRenderer::~ModelRenderer()
{
	if(scene)delete scene;
}

bool ModelRenderer::GetModelAsFBX(std::string & stream)
{
	Assimp::Exporter exporter;

	auto blob = exporter.ExportToBlob(scene, "fbx");

	if (blob == 0)
	{
		Log(LogLevel_Error, "Could not get exported blob for model with error %s", exporter.GetErrorString());
		return false;
	}

	stream.append((char*)blob->data, blob->size);

	return true;
}

bool ModelRenderer::LoadFileFromMemory(char * data, size_t length)
{
	Assimp::Importer importer;

	if (hasLoadedFile)
	{
		Log(LogLevel_Error, "Could Not Load Model From Memory Because a Different Model Was Already Loaded");
		return false;
	}

	importer.ReadFileFromMemory(data, length, 0);
	scene = importer.GetOrphanedScene();

	if (scene == 0)
	{
		Log(LogLevel_Error, "Could not Load Model FromMemory : %s", importer.GetErrorString());
		return false;
	}


	hasLoadedFile = true;
	NewModelLoaded();

	return true;
}

bool ModelRenderer::LoadFile(const char* file)
{
	Assimp::Importer importer;

	if (hasLoadedFile)
	{
		Log(LogLevel_Error,"Could Not Load Model %s Because a Different Model Was Already Loaded",file);
		return false;
	}

	importer.ReadFile(file, 0);
	scene = importer.GetOrphanedScene();

	if (scene)
	{
		hasLoadedFile = true;

		NewModelLoaded();
		return true;
	}
	else
	{
		Log(LogLevel_Error,"Could not Load Model %s : %s",file,importer.GetErrorString());
		return false;
	}
}

void ModelRenderer::ReleaseLoadedModel()
{
	if (hasLoadedFile)
	{
		if (scene)
		{
			delete scene;
			scene = 0;
		}
		else
		{
			Log(LogLevel_Warning,"ModelRenderer In Inconsistent State !!");
		}

		hasLoadedFile = false;
	}
}
