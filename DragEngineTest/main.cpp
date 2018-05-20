#include <memory>
#include <DragEngine.h>
#include "TestSceneObject.h"
#include <RenderEngineBase.h>

int main(int argc, char* argv[])
{
	DragEngine engine;
	engine.Init();

	TestSceneObject* object = new TestSceneObject();
	engine.GetRenderer()->AddSceneObject(object);

	engine.MainLoop();
}