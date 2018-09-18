#include <memory>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include "TestSceneObject.h"
#include "TestPlatformObject.h"
#include "TestCircleObject.h"
#include <Rendering/RenderEngineBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Core/2D/Components/RigidBody2DComponent.h>
#include <Core/2D/Named2DSceneObject.h>
#include <Utils/Random.h>
#include <Utils/HighPrecisionClock.h>
#include <algorithm>

#include <Core/Audio/AudioEngineBase.h>
#include <Core/Audio/common.h>

#include <Core/ThreadBase.h>

#include <string>

void TestSceneStuff();
void TestAudio();

static const unsigned TestNum = 1000;
static const unsigned SquareToSpawn = 1;
static const unsigned CircleToSpawn = 1;
static const float scale = 15.0f;

int main(int argc, char* argv[])
{
	TestSceneStuff();
}

void TestSceneStuff()
{
	ZoranEngine engine;
	engine.Init();
	engine.Setup2DScene(0, 0, 2000, 2000);

	std::string name_base = "TestSceneObject-";

	Vec2D CollisionPoint(0,0);
	for (int i = 0; i < SquareToSpawn; i++)
	{
		TestSceneObject* test = new TestSceneObject((name_base + std::to_string(i)));
		test->SetScale(scale, -scale);
		//test->SetPosition(Random::GetfloatInRange(-150, 150), Random::GetfloatInRange(-150, 150));
		//test->SetPosition(i*5 + -150,0);
		test->SetPosition({ -100,0 });
		test->GetPhysics()->StartSimulation();
		test->SetRotation(1);
		Vec2D pos = test->GetPosition();
		pos = CollisionPoint - pos;
		pos.normalize();
		test->GetPhysics()->SetGravity(pos*200);
		//test->GetPhysics2D()->SetGravity(pos*200);
		//test->GetPhysics2D()->ApplyForce(pos * 100);
		test->PreCaclModel();
		zEngine->AddSceneObject(test);
	}

	Vec2D gravity[2] = { Vec2D(0,600),Vec2D(0,-600) };

	TestSceneObject* sqr = 0;

	for (int i = 0; i < CircleToSpawn; i++)
	{
		TestCircleObject* test = new TestCircleObject(std::string("circle ") + std::to_string(i), 1.0);
		test->SetScale(scale, -scale);
		//test->SetPosition(i * 5 + -150, 100);
		//test->SetPosition(Random::GetfloatInRange(-150, 150), Random::GetfloatInRange(-150, 150));
		test->GetPhysics()->StartSimulation();
		test->SetPosition({100,0});
		//test->SetTarget(sqr);
		//sqr->SetTarget(test);
		//test->GetPhysics2D()->SetGravity(gravity[1]);
		//test->GetPhysics2D()->SetGravity(Vec2D(-600,0));
		Vec2D pos = test->GetPosition();
		pos = CollisionPoint - pos;
		pos.normalize();
		test->GetPhysics()->SetGravity(pos * 200);

		test->SetRotation(1.00);
		test->GetPhysics()->SetAngularVeloctiy(10);
		test->PreCaclModel();
		zEngine->AddSceneObject(test);
	}

	TestPlatformObject* platform = new TestPlatformObject("Ground");
	platform->SetScale(500, -50);
	platform->SetPosition(0, -250);
	platform->PreCaclModel();
	zEngine->AddSceneObject(platform);

	TestPlatformObject* platform2 = new TestPlatformObject("Ceiling");
	platform2->SetScale(500, -40);
	platform2->SetPosition(0, 250);
	platform2->PreCaclModel();
	zEngine->AddSceneObject(platform2);

	TestPlatformObject* platform3 = new TestPlatformObject("Left Wall");
	platform3->SetScale(40, -500);
	platform3->SetPosition(-500, 0);
	platform3->PreCaclModel();
	zEngine->AddSceneObject(platform3);

	TestPlatformObject* platform4 = new TestPlatformObject("Right Wall");
	platform4->SetScale(40, -500);
	platform4->SetPosition(500, 0);
	platform4->PreCaclModel();
	zEngine->AddSceneObject(platform4);

	//TestAudio();

	engine.SetPaused(true);
	engine.MainLoop();
}

void TestAudio()
{
	Named2DSceneObject * audioObj = new Named2DSceneObject("audio tester");
	AudioListener* listener = 0;
	SoundInstance* instance = 0;

	aEngine->CreateAudioListener(audioObj, &listener);
	aEngine->CreateSoundFromFile("test.wav",AFT_WAV,&instance);

	aEngine->PlaySoundInstance(instance);

	//ThreadBase::Sleep(0.5);

	//aEngine->StopSound(instance);
}