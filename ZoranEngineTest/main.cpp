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

#include <Core/WindowBase.h>

#include <ZGI/Windows/ZGIGameVirtualWindow.h>
#include <ZGI/Windows/ZGIDebugWindow.h>

#include <ZGI/Panels/ZGIWrapBoxPanel.h>
#include <ZGI/Panels/ZGIHorizontalBoxPanel.h>
#include <ZGI/Panels/ZGIVerticalBoxPanel.h>
#include <ZGI/Panels/ZGIUniformScalePanel.h>
#include <ZGI/Panels/ZGIFreeFormPanel.h>
#include <ZGI/Panels/ZGISwitcherPanel.h>
#include <ZGI/Panels/ZGIScrollPanel.h>
#include <ZGI/Panels/ZGIOverlayPanel.h>

#include <ZGI/Widgets/ZGIProgressBar.h>
#include <ZGI/Widgets/ZGISpinBox.h>
#include <ZGI/Widgets/ZGIComboBox.h>
#include <ZGI/Widgets/ZGICheckBox.h>
#include <ZGI/Widgets/ZGIImage.h>
#include <ZGI/Widgets/ZGISpacer.h>
#include <ZGI/Widgets/ZGILabel.h>
#include <ZGI/Widgets/ZGIButton.h>
#include <ZGI/Widgets/ZGITextEdit.h>

#include <ZGI/Core/ZGIBrush.h>

#include <Resources/FontResource.h>
#include <Resources/ResourceManager.h>

#include <Core/2D/TexturedSprite.h>

#include <Core/3D/StaticModelSceneObj.h>

#include <Resources/SoundAsset.h>
#include <Core/ThreadBase.h>

#include <string>

#include <Rendering/Primitives.h>

void Test2DSceneStuff();
void Test3DSceneStuff();
void TestGUIStuff();
void TestAudio();
void TestResourceStuff();

static const unsigned TestNum = 1000;
static const unsigned SquareToSpawn = 100;
static const unsigned CircleToSpawn = 100;
static const float scale = 40.0f;

int main(int argc, char* argv[])
{
	//Test2DSceneStuff();
	//Test3DSceneStuff();
	//TestGUIStuff();
	TestResourceStuff();
}

void Test2DSceneStuff()
{
	ZoranEngine engine;
	engine.Init();
	engine.Setup2DScene(0, 0, 2000, 2000);
	engine.CreateGameModeWindows(false);

	std::string name_base = "TestSceneObject-";

	/*TestSceneObject* test = engine.SpawnSceneObjectAtLocation<TestSceneObject>("scene",Vector2D(0,0), Vector2D(scale, -scale));
	TestCircleObject* circle = engine.SpawnSceneObjectAtLocation<TestCircleObject>("circle",Vector2D(0,0), Vector2D(scale, -scale));
	test->GetPhysics()->StartSimulation();
	circle->GetPhysics()->StartSimulation();*/

	Vector2D CollisionPoint(0,0);
	for (int i = 0; i < SquareToSpawn; i++)
	{
		TestSceneObject* test = engine.SpawnSceneObjectAtLocation<TestSceneObject>((name_base + std::to_string(i)),
			Vector2D(Random::GetfloatInRange(-500, 500), Random::GetfloatInRange(-300, 300)),Vector2D(scale, -scale));
		
		test->GetPhysics()->StartSimulation();
		test->SetRotation(1);
		
		Vector2D pos = test->GetPosition();
		pos = CollisionPoint - pos;
		pos.normalize();
		//test->GetPhysics()->SetGravity(pos*200);
		//test->GetPhysics2D()->SetGravity(pos*200);
		//test->GetPhysics2D()->ApplyForce(pos * 100);
	}

	Vector2D gravity[2] = { Vector2D(0,600),Vector2D(0,-600) };

	TestSceneObject* sqr = 0;

	for (int i = 0; i < CircleToSpawn; i++)
	{
		TestCircleObject* test = engine.SpawnSceneObjectAtLocation<TestCircleObject>(std::string("circle ") + std::to_string(i), Vector2D(Random::GetfloatInRange(-500, 500), 
			Random::GetfloatInRange(-300, 300)), Vector2D(scale,-scale));
		//test->SetPosition(i * 5 + -150, 100);
		//test->SetPosition({100,0});
		//test->SetTarget(sqr);
		//sqr->SetTarget(test);
		//test->GetPhysics2D()->SetGravity(gravity[1]);
		////test->GetPhysics2D()->SetGravity(Vec2D(-600,0));
		//Vector2D pos = test->GetPosition();
		//pos = CollisionPoint - pos;
		//pos.normalize();
		//test->GetPhysics()->SetGravity(pos * 200);

		test->GetPhysics()->StartSimulation();
		test->SetRotation(1.00);
		test->GetPhysics()->SetAngularVeloctiy(10);
	}

	engine.SpawnSceneObjectAtLocation<TestPlatformObject>("Ground", Vector2D(0, -500), Vector2D(1000, -50));
	engine.SpawnSceneObjectAtLocation<TestPlatformObject>("Ceiling", Vector2D(0, 500), Vector2D(1000, -50));
	engine.SpawnSceneObjectAtLocation<TestPlatformObject>("Left Wall", Vector2D(-950, 0), Vector2D(50, -1000));
	engine.SpawnSceneObjectAtLocation<TestPlatformObject>("Right Wall", Vector2D(950, 0), Vector2D(50, -1000));

	//TestAudio();

	engine.SetPaused(true);
	engine.MainLoop();
}

void Test3DSceneStuff()
{
	ZoranEngine engine;
	engine.Init();
	engine.Setup3DScene({}, { 1,1,1 }, 90, 1, 100);
	engine.CreateGameModeWindows(true);

	std::string name_base = "TestSceneObject-";

	StaticModelSceneObj* test = engine.SpawnSceneObjectAtLocation<StaticModelSceneObj>(name_base + "1", Vector3D(0,0,20), Vector3D(1.0f,1.0f,1.0f));
	test->SetMesh("teapot.zma");

	engine.MainLoop();
}

void TestGUIStuff()
{
	ZoranEngine engine;
	engine.Init();
	engine.Setup2DScene(0, 0, 2000, 2000);

	/*FontResource* r = ResourceManager::man()->FontForTTF("C:/Windows/Fonts/arial.ttf",64,4,Font_SDF_Type_MSDF);
	r->CreateBMPForASCII(ASCII);
	r->NormalizeGlyphs();
	r->SaveToFile("arial-msdf.zft");*/

	engine.CreateGameModeWindows(false);

	auto window = engine.GetMainWindow()->GetRootVirtualWindow();

	//auto w = window->SpawnWidget<ZGIWrapBoxPanel>(Vector2D(300,150));
	//auto p = window->SpawnWidget<ZGISwitcherPanel>();
	//auto p = window->SpawnWidget<ZGIOverlayPanel>();
	//auto s = window->SpawnWidget<ZGIScrollPanel>();
	//auto s = window->SpawnWidget<ZGIUniformScalePanel>();
	//auto p = window->SpawnWidget<ZGIVerticalBoxPanel>();
	//auto p = window->SpawnWidget<ZGIHorizontalBoxPanel>();
	auto p = window->SpawnWidget<ZGIFreeFormPanel>();

	//auto s = window->SpawnWidget<ZGISpinBox>(false, 0,0.5f);
	//auto c = window->SpawnWidget<ZGIComboBox>();

	//auto image1 = window->SpawnWidget<ZGIImage>();
	auto text = window->SpawnWidget<ZGITextEdit>();
	//auto spacer = window->SpawnWidget<ZGISpacer>();

	auto check = window->SpawnWidget<ZGICheckBox>();

	auto progress = window->SpawnWidget<ZGIProgressBar>();

	//image1->SetImage("test.png");
	//text->SetText("check box");

	//text->SetBoundsFromSize(true);
	//check->SetContent(text);

	//p->AddWidget(image1);
	//p->AddWidget(check);

	text->SetAlignment(Alignment_Center);

	text->SetAutoScaleFont(false);
	text->SetFontSize(100);

	//text->SetText(ASCII);
	text->SetText("This is a test\nThis is another test\nThis is a third test");
	//text->SetText("This is a very long setence with spaces and no new lines to test all the things with the font renderer class yippie ki yay");
	//text->SetText("This is a very long setence with spaces and new lines\nto test all the things\nwith the font renderer class\nyippie ki yay");

	window->SetRootContent(text);

	engine.MainLoop();
}

void TestAudio()
{
	/*Named2DSceneObject * audioObj = new Named2DSceneObject("audio tester");
	AudioListener* listener = 0;
	SoundInstance* instance = 0;

	aEngine->CreateAudioListener(audioObj, &listener);
	aEngine->CreateSoundFromFile("test.wav",AFT_WAV,&instance);

	aEngine->PlaySoundInstance(instance);*/

	//ThreadBase::Sleep(0.5);

	//aEngine->StopSound(instance);
}

void TestResourceStuff()
{
	ZoranEngine engine;
	engine.Init();
	engine.Setup2DScene(0, 0, 2000, 2000);

	//auto test = RM->MakeImageForPath("test.png","test");
	//auto circle = RM->MakeImageForPath("circle.png", "circle");

	//auto s = RM->MakeSoundForPath("test.wav", "test");
	auto s = RM->SoundForPath("test");
	s->SaveToFile("test");
}
