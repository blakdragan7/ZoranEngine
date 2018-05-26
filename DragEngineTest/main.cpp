#include <memory>
#include <DragEngine.h>
#include <PhysicsEngine.h>
#include "TestSceneObject.h"
#include <RenderEngineBase.h>

int main(int argc, char* argv[])
{
	DragEngine engine;
	engine.Init();
	engine.GetPhysicsEngine()->SetupFor2D(Vector3D(0,0,0),Vector3D(2,2,1));
	
	TestSceneObject* object = new TestSceneObject();
	object->SetScale(0.5, -0.5, 1);
	object->SetPosition(Vector3D(1,0,0));
	engine.GetRenderer()->AddSceneObject(object);

	TestSceneObject* otherObject = new TestSceneObject();
	otherObject->SetScale(0.5, -0.5, 1);
	otherObject->SetPosition(Vector3D(-1, 0, 0));
	engine.GetRenderer()->AddSceneObject(otherObject);

	engine.MainLoop();
}