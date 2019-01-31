#pragma once
#include <Rendering/RenderedObjectBase.h>


struct aiScene;
class ZoranEngine_EXPORT ModelRenderer : public RenderedObjectBase
{
private:
	bool hasLoadedFile;

protected:
	const aiScene* scene;

protected:
	virtual void NewModelLoaded() = 0;

public:
	ModelRenderer(PrimitiveType pt, DrawType dt);
	ModelRenderer(const char* file, PrimitiveType pt, DrawType dt);

	~ModelRenderer();

	bool GetModelAsFBX(std::string& stream);

	bool LoadFileFromMemory(char* data, size_t length);
	bool LoadFile(const char* file);
	void ReleaseLoadedModel();

	inline bool GetHasLoadedFile() { return hasLoadedFile; }

	//virtual void RenderModel() = 0;
};

