#include "stdafx.h"
#include "OpenGLModelRenderer.h"
#include <ThirdParty/assimp/scene.h>
#include <Rendering/OpenGL/OpenGLVertexGroup.h>
#include <Rendering/OpenGL/OpenGLBuffer.h>

#include <Math/Vector2.h>

#include <GL/glew.h>

OpenGLModelRenderer::OpenGLModelRenderer(OpenGLContext* context) : context(context), ModelRenderer(PT_Unused,DT_Dynamic)
{
	vertexGroup = new OpenGLVertexGroup(GL_TRIANGLES,context);
}

OpenGLModelRenderer::~OpenGLModelRenderer()
{
	delete vertexGroup;
}

void OpenGLModelRenderer::NewModelLoaded()
{
	std::vector<Vector3D> vertecies;
	std::vector<Vector3D> normals;
	std::vector<Vector2D> uvs;

	std::vector<unsigned> indecies;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		for (unsigned j = 0; j < mesh->mNumVertices; j++)
		{
			vertecies.push_back({ mesh->mVertices[j].x,mesh->mVertices[j].y,mesh->mVertices[j].z });
			normals.push_back({ mesh->mNormals[j].x,mesh->mNormals[j].y,mesh->mNormals[j].z });
			//uvs.push_back({ mesh->mTextureCoords[j]->x, mesh->mTextureCoords[j]->y });
		}

		for (unsigned j = 0; j < mesh->mNumFaces; j++)
		{
			for (unsigned k = 0; k < mesh->mFaces[j].mNumIndices; k++)
			{
				indecies.push_back(mesh->mFaces[j].mIndices[k]);
			}
		}
	}

	OpenGLBuffer* vertBuffer = new OpenGLBuffer(GL_ARRAY_BUFFER, (void*)&vertecies.at(0), 3, GL_STATIC_DRAW, vertecies.size() * sizeof(Vector3D), context);
	OpenGLBuffer* normalBuffer = new OpenGLBuffer(GL_ARRAY_BUFFER, (void*)&normals.at(0), 3, GL_STATIC_DRAW, normals.size() * sizeof(Vector3D), context);
	//OpenGLBuffer* uvBuffer = new OpenGLBuffer(GL_ARRAY_BUFFER, (void*)&uvs.at(0), 2, GL_STATIC_DRAW, uvs.size() * sizeof(Vector2D), context);
	OpenGLBuffer* indecieBuffer = new OpenGLBuffer(GL_ELEMENT_ARRAY_BUFFER, (void*)&indecies.at(0),1,GL_STATIC_DRAW, indecies.size() * sizeof(unsigned),context);

	vertexGroup->AddBufferForAttr(0, vertBuffer);
	vertexGroup->AddBufferForAttr(2, normalBuffer);
	//vertexGroup->AddBufferForAttr(1, uvBuffer);

	vertexGroup->SetElementBuffer(indecieBuffer);
	vertexGroup->SetNumVerts(static_cast<unsigned>(indecies.size()));
}

void OpenGLModelRenderer::RenderObject()
{
	vertexGroup->RenderObject();
}
