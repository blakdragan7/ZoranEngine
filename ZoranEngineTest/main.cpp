#include <memory>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include "TestSceneObject.h"
#include "TestPlatformObject.h"
#include <Rendering/RenderEngineBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>

#include <Utils/Random.h>
#include <string>
int main(int argc, char* argv[])
{
	ZoranEngine engine;
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
		zEngine->AddSceneObject(test);
	}*/

	TestSceneObject* test = new TestSceneObject("Dynamic");
	test->SetRotation(1);
	test->SetScale(20, -20);
	test->SetPosition(-400, 200);
	test->GetPhysics2D()->StartPhysicsSim();
	test->GetPhysics2D()->SetGravity(Vec2D(0, -400));
	test->GetPhysics2D()->ApplyForce(Vec2D(-200,600));
	test->GetPhysics2D()->SetSweptCollision(false);
	test->GetPhysics2D()->SetRestitution(1);
	zEngine->AddSceneObject(test);
	
	TestPlatformObject* platform = new TestPlatformObject("Ground");
	platform->SetScale(500, -50);
	platform->SetPosition(0, -500);
	zEngine->AddSceneObject(platform);

	TestPlatformObject* platform2 = new TestPlatformObject("Ceiling");
	platform2->SetScale(500, -40);
	platform2->SetPosition(0, 500);
	zEngine->AddSceneObject(platform2);

	TestPlatformObject* platform3 = new TestPlatformObject("Left Wall");
	platform3->SetScale(40, -500);
	platform3->SetPosition(-500, 0);
	zEngine->AddSceneObject(platform3);

	TestPlatformObject* platform4 = new TestPlatformObject("Right Wall");
	platform4->SetScale(40, -500);
	platform4->SetPosition(500, 0);
	zEngine->AddSceneObject(platform4);

	engine.MainLoop();
}