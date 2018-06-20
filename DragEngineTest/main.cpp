#include <memory>
#include <DragEngine.h>
#include <PhysicsEngine.h>
#include "TestSceneObject.h"
#include "TestPlatformObject.h"
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
	test->SetPosition(-400, 200, 0);
	test->GetPhysics()->StartPhysicsSim();
	test->GetPhysics()->SetGravity(Vec3D(0, -400, 0));
	test->GetPhysics()->ApplyForce(Vec3D(200,-100,0));
	test->GetPhysics()->SetSweptCollision(false);
	test->GetPhysics()->SetRestitution(1);
	dEngine->AddSceneObject(test);
	
	TestPlatformObject* platform = new TestPlatformObject();
	platform->SetScale(500, -50, 1);
	platform->SetPosition(0, -500, 0);
	dEngine->AddSceneObject(platform);

	TestPlatformObject* platform2 = new TestPlatformObject();
	platform2->SetScale(500, -40, 1);
	platform2->SetPosition(0, 500, 0);
	dEngine->AddSceneObject(platform2);

	TestPlatformObject* platform3 = new TestPlatformObject();
	platform3->SetScale(40, -500, 1);
	platform3->SetPosition(-500, 0, 0);
	dEngine->AddSceneObject(platform3);

	TestPlatformObject* platform4 = new TestPlatformObject();
	platform4->SetScale(40, -500, 1);
	platform4->SetPosition(500, 0, 0);
	dEngine->AddSceneObject(platform4);

	engine.MainLoop();
}