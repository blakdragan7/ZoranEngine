#pragma once
#include <Rendering/RenderedObjectBase.h>

namespace Assimp
{
	class Importer;
}

struct aiScene;
class ZoranEngine_EXPORT ModelRenderer : public RenderedObjectBase
{
private:
	bool hasLoadedFile;
	Assimp::Importer* importer;

protected:
	const aiScene* scene;

protected:
	virtual void NewModelLoaded() = 0;

public:
	ModelRenderer(PrimitiveType pt, DrawType dt);
	ModelRenderer(const char* file, PrimitiveType pt, DrawType dt);

	~ModelRenderer();

	void LoadFile(const char* file);
	void ReleaseLoadedModel();

	inline bool GetHasLoadedFile() { return hasLoadedFile; }

	//virtual void RenderModel() = 0;
};

