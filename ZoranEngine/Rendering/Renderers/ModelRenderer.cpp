#include "stdafx.h"
#include "ModelRenderer.h"

#include <ThirdParty/assimp/scene.h>
#include <ThirdParty/assimp/Importer.hpp>

ModelRenderer::ModelRenderer(PrimitiveType pt, DrawType dt) : scene(0), hasLoadedFile(false), RenderedObjectBase(pt, VT_Float, dt)
{
	importer = new Assimp::Importer();
}

ModelRenderer::ModelRenderer(const char* file, PrimitiveType pt, DrawType dt) : RenderedObjectBase(pt, VT_Float, dt)
{
	LoadFile(file);
}

ModelRenderer::~ModelRenderer()
{
	
}

void ModelRenderer::LoadFile(const char* file)
{
	if (hasLoadedFile)
	{
		Log(LogLevel_Error,"Could Not Load Model %s Because a Different Model Was Already Loaded !!\n",file);
		return;
	}

	scene = importer->ReadFile(file, 0);

	//scene = importer.GetOrphanedScene();
	//Log(LogLevel_Warning, "Using GetOrpganedScene From Assimp Importer which is warned to be an issue on platforms other then windows !! \n");

	if (scene)
	{
		hasLoadedFile = true;

		NewModelLoaded();
	}
	else
	{
		Log(LogLevel_Error,"Could not Load Model %s : %s \n",file,importer->GetErrorString());
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
			Log(LogLevel_Warning,"ModelRenderer In Inconsistent State !!\n");
		}

		hasLoadedFile = false;
	}
}
