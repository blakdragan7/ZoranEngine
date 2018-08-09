#include <memory>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include "TestSceneObject.h"
#include "TestPlatformObject.h"
#include "TestCircleObject.h"
#include <Rendering/RenderEngineBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>

#include <Core/Containers/ZArray.h>
#include <Core/Containers/ZLinkedList.h>
#include <Core/Containers/ZDoubleLinkedList.h>
#include <Core/Containers/ZDictionary.h>

#include <Utils/Random.h>
#include <Utils/HighPrecisionClock.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

void TestArrayStuff();
void TestSceneStuff();
void TestLinkedListStuff();
void TestDoubleLinkedListStuff();
void TestDictionaryStuff();

static const unsigned TestNum = 1000;

int main(int argc, char* argv[])
{

	//TestDictionaryStuff();
	//TestDoubleLinkedListStuff();
	//TestLinkedListStuff();
	//TestSceneStuff();
	//TestArrayStuff();

	std::cin.get();
}

void TestDictionaryStuff()
{
	HighPrecisionClock cl;

	ZDictionary<std::string, std::string> dict;

	static const std::string vStart = "sss ";
	for (unsigned i = 0; i < TestNum; i++)
	{
		dict.Add(vStart + std::to_string(i), vStart + std::to_string(i));
	}

	for (unsigned i = TestNum - 1; i > TestNum / 2; i--)
	{
		dict.Remove(vStart + std::to_string(i));
	}

	for (unsigned i = 0; i < TestNum/2.0; i++)
	{
		std::string val = dict[vStart + std::to_string(i)];
	}
	
	double tm = cl.GetDiffSeconds();

	std::cout << "ZDictionary Took " << tm << " seconds\n";

	cl.TakeClock();

	double nl = cl.GetDiffSeconds();

	std::cout << "map Took " << nl << " seconds\n";

	std::cout << "ZDictionary ratio is " << (nl / tm) << std::endl;
}

void TestDoubleLinkedListStuff()
{
	HighPrecisionClock cl;

	ZDoubleLinkedList<unsigned> ints;

	for (unsigned i = 0; i < TestNum; i++)
	{
		ints.Add(i);
	}

	for (unsigned i = TestNum - 1; i > TestNum / 2; i--)
	{
		ints.PopLast();
	}

	double tm = cl.GetDiffSeconds();

	std::cout << "ZDoubleLinkedList Took " << tm << " seconds\n";

	cl.TakeClock();

	std::vector<unsigned> v;

	for (unsigned i = 0; i < TestNum; i++)
	{
		v.push_back(i);
	}

	for (unsigned i = TestNum - 1; i > TestNum / 2; i--)
	{
		v.erase(std::find(v.begin(), v.end(), i));
	}

	double nl = cl.GetDiffSeconds();

	std::cout << "Vector Took " << nl << " seconds\n";

	std::cout << "ZDoubleLinkedList ratio is " << (nl / tm) << std::endl;
}

void TestLinkedListStuff()
{

	HighPrecisionClock cl;

	ZLinkedList<unsigned> ints;

	for (unsigned i = 0; i < TestNum; i++)
	{
		ints.Add(i);
	}

	for (unsigned i = TestNum - 1; i > TestNum / 2; i--)
	{
		ints.PopLast();
	}

	double tm = cl.GetDiffSeconds();

	std::cout << "ZLinkedList Took " << tm << " seconds\n";

	cl.TakeClock();

	std::vector<unsigned> v;

	for (unsigned i = 0; i < TestNum; i++)
	{
		v.push_back(i);
	}

	for (unsigned i = TestNum - 1; i > TestNum / 2; i--)
	{
		v.erase(std::find(v.begin(), v.end(), i));
	}

	double nl = cl.GetDiffSeconds();

	std::cout << "Vector Took " << nl << " seconds\n";

	std::cout << "ZLinkedList ratio is " << (nl / tm) << std::endl;
}

void TestArrayStuff()
{
	HighPrecisionClock cl;

	ZArray<std::string> ints;

	for (unsigned i = 0; i < TestNum; i++)
	{
		ints.Add(std::to_string(i));
	}

	for (unsigned i = TestNum - 1; i > TestNum / 2; i--)
	{
		ints.RemoveAt(i);
	}

	double tm = cl.GetDiffSeconds();

	std::cout << "ZArray Took " << tm << " seconds\n";

	cl.TakeClock();

	std::vector<std::string> v;

	for (unsigned i = 0; i < TestNum; i++)
	{
		v.push_back(std::to_string(i));
	}

	for (unsigned i = TestNum - 1; i > TestNum / 2; i--)
	{
		v.erase(v.begin() + i);
		//v.erase(std::find(v.begin(), v.end(), i));
	}

	double nl = cl.GetDiffSeconds();

	std::cout << "Vector Took " << nl << " seconds\n";

	std::cout << "ZArray ratio is " << (nl / tm) << std::endl;

	ints.Empty();
}

void TestSceneStuff()
{
	ZoranEngine engine;
	engine.Init();
	engine.Setup2DScene(0, 0, 2000, 2000);

	std::string name_base = "TestSceneObject-";

	Vec2D CollisionPoint(0,0);
	for (unsigned i = 0; i < 50; i++)
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
	}

	Vec2D gravity[2] = { Vec2D(0,600),Vec2D(0,-600) };

	TestSceneObject* sqr = 0;

	/*for (int i = 0; i < 1; i++)
	{
		TestSceneObject* test = new TestSceneObject(std::string("dynamic ") + std::to_string(i));
		test->SetScale(20.0f, -20.0f);
		Vec2D pos(0, -100 + ((float)i * 60));
		test->SetPosition(pos);
		test->GetPhysics()->StartPhysicsSim();

		pos = -pos;
		pos.normalize();

		//test->GetPhysics2D()->SetGravity(gravity[0]);
		//test->GetPhysics2D()->SetGravity(Vec2D(-500,0));
		//test->GetPhysics2D()->SetGravity(pos * 100);
		//test->GetPhysics2D()->ApplyForce(Vec2D(100,800));
		test->GetPhysics2D()->SetSweptCollision(false);
		test->SetRotation(1.00);
		zEngine->AddSceneObject(test);

		sqr = test;
	}

	for (int i = 0; i < 1; i++)
	{
		TestCircleObject* test = new TestCircleObject(std::string("circle ") + std::to_string(i), 1.0);
		test->SetScale(20.0f, -20.0f);
		Vec2D pos(0, -100 + ((float)2 * 60));
		test->SetPosition(pos);
		test->GetPhysics()->StartPhysicsSim();
		test->SetTarget(sqr);
		sqr->SetTarget(test);
		//test->GetPhysics2D()->SetGravity(gravity[1]);
		//test->GetPhysics2D()->SetGravity(Vec2D(-600,0));
		test->GetPhysics2D()->SetSweptCollision(false);
		//test->SetRotation(1.00);
		test->GetPhysics2D()->SetAngularVeloctiy(10);
		zEngine->AddSceneObject(test);
	}*/

	TestPlatformObject* platform = new TestPlatformObject("Ground");
	platform->SetScale(500, -50);
	platform->SetPosition(0, -250);
	zEngine->AddSceneObject(platform);

	TestPlatformObject* platform2 = new TestPlatformObject("Ceiling");
	platform2->SetScale(500, -40);
	platform2->SetPosition(0, 250);
	zEngine->AddSceneObject(platform2);

	TestPlatformObject* platform3 = new TestPlatformObject("Left Wall");
	platform3->SetScale(40, -500);
	platform3->SetPosition(-500, 0);
	zEngine->AddSceneObject(platform3);

	TestPlatformObject* platform4 = new TestPlatformObject("Right Wall");
	platform4->SetScale(40, -500);
	platform4->SetPosition(500, 0);
	zEngine->AddSceneObject(platform4);

	engine.SetPaused(true);
	engine.MainLoop();
}