#include "stdafx.h"
#include "DebugShader2D.h"
#include "Core/CameraBase.h"
#include "GL/glew.h"
#include "Core/SceneObject.h"
#include <Core/2D/DebugSceneObject2D.h>

#include <Utils/HighPrecisionClock.h>

DebugShader2D::DebugShader2D(DebugSceneObject2D* object)
{
	debugObject = object;
	AddShaderFromSource("Shaders/2D/standard_debug.vert", GL_VERTEX_SHADER);
	AddShaderFromSource("Shaders/2D/standard_debug.frag", GL_FRAGMENT_SHADER);
	Link();
}

DebugShader2D::~DebugShader2D()
{
}

bool DebugShader2D::SetupShaderFromSceneObject(SceneObject * object)
{
	Matrix44 model = object->GetModel();

	model = zEngine->GetCamera()->GetModel() * model;
	setUniformMat4("MVP",&model[0]);
	setUniform("Color", &debugObject->GetColor());

	return true;
}