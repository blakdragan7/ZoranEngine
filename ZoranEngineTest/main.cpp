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

	Vec2D CollisionPoint(0,0);
	for (unsigned i = 0; i < 10; i++)
	{
		TestSceneObject* test = new TestSceneObject((name_base + std::to_string(i)));
		test->SetScale(40, -40);
		test->SetPosition(Random::GetDoubleInRange(-300, 300), Random::GetDoubleInRange(-300, 300));
		test->GetPhysics()->StartPhysicsSim();
		Vec2D pos = test->GetPosition();
		pos = CollisionPoint - pos;
		pos.normalize();
		zEngine->AddSceneObject(test);
	}
	
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