#include <memory>
#include <DragEngine.h>
#include <PhysicsEngine.h>
#include "TestSceneObject.h"
#include <RenderEngineBase.h>
#include <PhysicsObjectBase.h>

#include <Random.h>
#include <string>
int main(int argc, char* argv[])
{
	DragEngine engine;
	engine.Init();
	engine.Setup2DScene(0, 0, 2000, 2000);
	
	std::string name_base = "TestSceneObject-";

	Vec3D CollisionPoint(0,0,0);

	/*for (unsigned i = 0; i < 100; i++)
	{
		TestSceneObject* test = new TestSceneObject((name_base + std::to_string(i)));
		test->SetScale(0.02, -0.02, 1);
		test->SetPosition(Random::GetDoubleInRange(-1, 1), Random::GetDoubleInRange(-1, 1),0);
		test->GetPhysics()->StartPhysicsSim();
		Vec3D pos = test->GetPosition();
		pos = CollisionPoint - pos;
		pos.normalize();
		test->GetPhysics()->SetGravity(pos * 0.01);
		dEngine->AddSceneObject(test);
	}*/

	TestSceneObject* test = new TestSceneObject("top");
	test->SetScale(20, -20, 1);
	test->SetPosition(0, 500, 0);
	test->GetPhysics()->StartPhysicsSim();
	test->GetPhysics()->SetGravity(Vec3D(0,-9.8,0));
	dEngine->AddSceneObject(test);

	TestSceneObject* test2 = new TestSceneObject("bottom");
	test2->SetScale(20, -20, 1);
	test2->SetPosition(200, -500, 0);
	test2->GetPhysics()->StartPhysicsSim();
	Vec3D pos = test2->GetPosition();
	pos = CollisionPoint - pos;
	pos.normalize();
	test2->GetPhysics()->SetGravity(pos*8);
	dEngine->AddSceneObject(test2);
	
	engine.MainLoop();
}