#include "stdafx.h"
#include "DebugShader2D.h"
#include "Core/CameraBase.h"
#include "GL/glew.h"
#include "Core/SceneObject.h"
#include <Core/2D/DebugSceneObject2D.h>

#include <Utils/HighPrecisionClock.h>

const ShaderInitMap* DebugShader2D::initMap = (new ShaderInitMap(
{
	{ GL_VERTEX_SHADER, "Shaders/2D/standard_debug.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/2D/standard_debug.frag" }
}));

DebugShader2D::DebugShader2D(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}

DebugShader2D::~DebugShader2D()
{
}
