#include "stdafx.h"
#include "TestSceneObject.h"
#include "StandardShader.h"
#include "OpenGLObject.h"
TestSceneObject::TestSceneObject()
{
	SetShaderProgram(new StandardShader());
	OpenGLObject* object = dynamic_cast<OpenGLObject*>(GetRenderedObject());

	if (object)
	{
		object->MakeFullScreenQuad();
	}
}


TestSceneObject::~TestSceneObject()
{
}
