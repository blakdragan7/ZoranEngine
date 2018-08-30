#include "stdafx.h"
#include "DebugShader2D.h"
#include "Core/CameraBase.h"
#include "GL/glew.h"
#include "Core/SceneObject.h"
#include <Core/2D/DebugSceneObject2D.h>

#include <Utils/HighPrecisionClock.h>

const ShaderInitMap DebugShader2D::initMap =
{
	{GL_VERTEX_SHADER, "Shaders/2D/standard_debug.vert" },
	{GL_FRAGMENT_SHADER, "Shaders/2D/standard_debug.frag"}
};

DebugShader2D::DebugShader2D(ShaderInitMap initMap) : OpenGLShaderProgramBase(initMap)
{
}

DebugShader2D::~DebugShader2D()
{
}

void DebugShader2D::SetModelMatrix(const Matrix44 & world)
{
	Matrix44 model(false);
	model = zEngine->GetCamera()->GetModel() * world;
	setUniformMat4("MVP", &model[0]);
}
