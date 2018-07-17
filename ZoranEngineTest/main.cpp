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

	/*Vec2D CollisionPoint(0,0);
	for (unsigned i = 0; i < 100; i++)
	{
		TestSceneObject* test = new TestSceneObject((name_base + std::to_string(i)));
		test->SetScale(40, -40);
		test->SetPosition(Random::GetfloatInRange(-300, 300), Random::GetfloatInRange(-300, 300));
		test->GetPhysics()->StartPhysicsSim();
		test->GetPhysics2D()->SetSweptCollision(false);
		test->SetRotation(1);
		Vec2D pos = test->GetPosition();
		pos = CollisionPoint - pos;
		pos.normalize();
		test->GetPhysics2D()->SetGravity(pos*200);
		//test->GetPhysics2D()->ApplyForce(pos * 100);
		zEngine->AddSceneObject(test);
	}*/

	Vec2D gravity[2] = { Vec2D(0,600),Vec2D(0,-600) };

	for (int i = 0; i < 2; i++)
	{
		TestSceneObject* test = new TestSceneObject(std::string("dynamic ") + std::to_string(i));
		test->SetScale(40.0f, -40.0f);
		Vec2D pos(0, -300.0f + ((float)i * 600.0f));
		test->SetPosition(pos);
		test->GetPhysics()->StartPhysicsSim();

		pos = -pos;
		pos.normalize();

		test->GetPhysics2D()->SetGravity(gravity[i]);
		//test->GetPhysics2D()->SetGravity(Vec2D(0,-980));
		//test->GetPhysics2D()->ApplyForce(Vec2D(100,800));
		test->GetPhysics2D()->SetSweptCollision(false);
		test->SetRotation(0);
		zEngine->AddSceneObject(test);
	}
	
	/*TestPlatformObject* platform = new TestPlatformObject("Ground");
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
	zEngine->AddSceneObject(platform4);*/

	engine.SetPaused(true);
	engine.MainLoop();
}