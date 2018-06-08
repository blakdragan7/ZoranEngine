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
	engine.GetPhysicsEngine()->SetupFor2D(Vector3D(0,0,0),Vector3D(2,2,10));
	
	std::string name_base = "TestSceneObject-";

	Vec3D CollisionPoint(0,0,0);

	for (unsigned i = 0; i < 600; i++)
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
	}

	/*TestSceneObject* object1 = new TestSceneObject();
	object1->SetScale(0.1, 0.1, 1);
	object1->SetPosition(-0.6,1,1);

	TestSceneObject* object2 = new TestSceneObject();
	object2->SetScale(0.1, 0.1, 1);
	object2->SetPosition(-0.8, 1, 1);

	TestSceneObject* object3 = new TestSceneObject();
	object3->SetScale(0.1, 0.1, 1);
	object3->SetPosition(0.6, 1, 1);

	TestSceneObject* object4 = new TestSceneObject();
	object4->SetScale(0.1, 0.1, 1);
	object4->SetPosition(0.8, 1, 1);

	TestSceneObject* object5 = new TestSceneObject();
	object5->SetScale(0.1, 0.1, 1);
	object5->SetPosition(-0.6, -1, 1);

	TestSceneObject* object6 = new TestSceneObject();
	object6->SetScale(0.1, 0.1, 1);
	object6->SetPosition(-0.8, -1, 1);

	TestSceneObject* object7 = new TestSceneObject();
	object7->SetScale(0.1, 0.1, 1);
	object7->SetPosition(0.6, -1, 1);
	
	TestSceneObject* object8 = new TestSceneObject();
	object8->SetScale(0.1, 0.1, 1);
	object8->SetPosition(0.8, -1, 1);
	*/
	/*Vec3D CollisionPoint(-0.8, 0, 1);

	TestSceneObject* object7 = new TestSceneObject("TopStart");
	object7->SetScale(0.1, -0.1, 1);
	object7->SetPosition(-0.8, 0.8, 1);

	Vec3D pos = object7->GetPosition();
	pos = CollisionPoint - pos;
	pos.normalize();
	object7->GetPhysics()->SetGravity(pos*0.01);

	TestSceneObject* object8 = new TestSceneObject("TopMiddleStart");
	object8->SetScale(0.1, -0.1, 1);
	object8->SetPosition(-0.8, 0.3, 1);

	pos = object8->GetPosition();
	pos = CollisionPoint - pos;
	pos.normalize();
	object8->GetPhysics()->SetGravity(pos*0.01);

	TestSceneObject* object9 = new TestSceneObject("bottomMiddleStart");
	object9->SetScale(0.1, -0.1, 1); 
	object9->SetPosition(-0.8, -0.3, 1);

	pos = object9->GetPosition();
	pos = CollisionPoint - pos;
	pos.normalize();
	object9->GetPhysics()->SetGravity(pos*0.01);

	TestSceneObject* object10 = new TestSceneObject("BottomStart");
	object10->SetScale(0.1, -0.1, 1);
	object10->SetPosition(-0.8, -0.8, 1);

	pos = object10->GetPosition();
	pos = CollisionPoint - pos;
	pos.normalize();
	object10->GetPhysics()->SetGravity(pos*0.01);

	dEngine->AddSceneObject(object7);
	dEngine->AddSceneObject(object8);
	dEngine->AddSceneObject(object9);
	dEngine->AddSceneObject(object10);


	object7->GetPhysics()->StartPhysicsSim();
	object8->GetPhysics()->StartPhysicsSim();
	object9->GetPhysics()->StartPhysicsSim();
	object10->GetPhysics()->StartPhysicsSim();
	*/
	engine.MainLoop();
}