#include <memory>
#include <DragEngine.h>
#include <PhysicsEngine.h>
#include "TestSceneObject.h"
#include <RenderEngineBase.h>
#include <PhysicsObjectBase.h>

#include <Random.h>

int main(int argc, char* argv[])
{
	DragEngine engine;
	engine.Init();
	engine.GetPhysicsEngine()->SetupFor2D(Vector3D(0,0,0),Vector3D(2,2,10));
	
	for (unsigned i = 0; i < 32; i++)
	{
		TestSceneObject* test = new TestSceneObject();
		test->SetScale(0.02, -0.02, 1);
		test->SetPosition(Random::GetDoubleInRange(-1,1), Random::GetDoubleInRange(-1, 1),0);
		test->GetPhysics()->StartPhysicsSim();
		Vec3D pos = test->GetPosition();
		pos.normalize();
		test->GetPhysics()->SetGravity(-pos * 0.01);
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

	TestSceneObject* object7 = new TestSceneObject();
	object7->SetScale(0.1, -0.1, 1);
	object7->SetPosition(-1, 0, 1);
	object7->GetPhysics()->SetGravity(Vec3D(0.0098,0,0));

	TestSceneObject* object8 = new TestSceneObject();
	object8->SetScale(0.1, -0.1, 1);
	object8->SetPosition(1, 0, 1);
	object8->GetPhysics()->SetGravity(Vec3D(-0.0098, 0, 0));

	TestSceneObject* object9 = new TestSceneObject();
	object9->SetScale(0.1, -0.1, 1); 
	object9->SetPosition(0, -1, 1);
	object9->GetPhysics()->SetGravity(Vec3D(0, 0.0098, 0));

	TestSceneObject* object10 = new TestSceneObject();
	object10->SetScale(0.1, -0.1, 1);
	object10->SetPosition(0, 1, 1);

	object7->GetPhysics()->StartPhysicsSim();
	object8->GetPhysics()->StartPhysicsSim();
	object9->GetPhysics()->StartPhysicsSim();
	object10->GetPhysics()->StartPhysicsSim();*/

	engine.MainLoop();
}