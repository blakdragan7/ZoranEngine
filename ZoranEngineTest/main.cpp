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

#include <Core/Resources/FontResource.h>
#include <Core/Resources/ResourceManager.h>

#include <Rendering/TextureManager.h>

#include <Core/2D/TexturedSprite.h>

#include <Core/3D/StaticModelSceneObj.h>

#include <Core/Audio/AudioEngineBase.h>
#include <Core/Audio/common.h>

#include <Core/ThreadBase.h>

#include <string>

#include <Rendering/Primitives.h>

void Test2DSceneStuff();
void Test3DSceneStuff();
void TestGUIStuff();
void TestAudio();

static const unsigned TestNum = 1000;
static const unsigned SquareToSpawn = 100;
static const unsigned CircleToSpawn = 100;
static const float scale = 40.0f;

int main(int argc, char* argv[])
{
	//Test2DSceneStuff();
	//Test3DSceneStuff();
	TestGUIStuff();
}

void Test2DSceneStuff()
{
	ZoranEngine engine;
	engine.Init();
	engine.Setup2DScene(0, 0, 2000, 2000);
	engine.CreateGameModeWindows(false);

	std::string name_base = "TestSceneObject-";

	Vector2D CollisionPoint(0,0);
	for (int i = 0; i < SquareToSpawn; i++)
	{
		TestSceneObject* test = new TestSceneObject((name_base + std::to_string(i)));
		test->SetScale(scale, -scale);
		test->SetPosition(Random::GetfloatInRange(-500, 500), Random::GetfloatInRange(-300, 300));
		//test->SetPosition(i*5 + -150,0);
		//test->SetPosition({ -100,0 });
		test->GetPhysics()->StartSimulation();
		test->SetRotation(1);
		Vector2D pos = test->GetPosition();
		pos = CollisionPoint - pos;
		pos.normalize();
		//test->GetPhysics()->SetGravity(pos*200);
		//test->GetPhysics2D()->SetGravity(pos*200);
		//test->GetPhysics2D()->ApplyForce(pos * 100);
		test->PreCaclModel();
		zEngine->AddSceneObject(test);
	}

	Vector2D gravity[2] = { Vector2D(0,600),Vector2D(0,-600) };

	TestSceneObject* sqr = 0;

	for (int i = 0; i < CircleToSpawn; i++)
	{
		TestCircleObject* test = new TestCircleObject(std::string("circle ") + std::to_string(i), 1.0);
		test->SetScale(scale, -scale);
		//test->SetPosition(i * 5 + -150, 100);
		test->SetPosition(Random::GetfloatInRange(-500, 500), Random::GetfloatInRange(-300, 300));
		test->GetPhysics()->StartSimulation();
		//test->SetPosition({100,0});
		//test->SetTarget(sqr);
		//sqr->SetTarget(test);
		//test->GetPhysics2D()->SetGravity(gravity[1]);
		//test->GetPhysics2D()->SetGravity(Vec2D(-600,0));
		Vector2D pos = test->GetPosition();
		pos = CollisionPoint - pos;
		pos.normalize();
		//test->GetPhysics()->SetGravity(pos * 200);

		test->SetRotation(1.00);
		test->GetPhysics()->SetAngularVeloctiy(10);
		test->PreCaclModel();
		zEngine->AddSceneObject(test);
	}

	TestPlatformObject* platform = new TestPlatformObject("Ground");
	platform->SetScale(1000, -50);
	platform->SetPosition(0, -500);
	platform->PreCaclModel();
	zEngine->AddSceneObject(platform);

	TestPlatformObject* platform2 = new TestPlatformObject("Ceiling");
	platform2->SetScale(1000, -50);
	platform2->SetPosition(0, 500);
	platform2->PreCaclModel();
	zEngine->AddSceneObject(platform2);

	TestPlatformObject* platform3 = new TestPlatformObject("Left Wall");
	platform3->SetScale(50, -1000);
	platform3->SetPosition(-950, 0);
	platform3->PreCaclModel();
	zEngine->AddSceneObject(platform3);

	TestPlatformObject* platform4 = new TestPlatformObject("Right Wall");
	platform4->SetScale(50, -1000);
	platform4->SetPosition(950, 0);
	platform4->PreCaclModel();
	zEngine->AddSceneObject(platform4);

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

	StaticModelSceneObj* test = new StaticModelSceneObj(name_base + "1");

	test->SetPosition(0, 0, 20);
	test->SetScale({ 1.0f,1.0f,1.0f });
	test->SetMesh("teapot.obj");
	test->PreCaclModel();

	engine.AddSceneObject(test);

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

	//auto w = new ZGIWrapBoxPanel({300,150}, window);
	//auto p = new ZGISwitcherPanel(window);
	//auto p = new ZGIOverlayPanel(window);
	//auto s = new ZGIScrollPanel(window);
	//auto s = new ZGIUniformScalePanel(window);
	//auto p = new ZGIVerticalBoxPanel(window);
	//auto p = new ZGIHorizontalBoxPanel(window);
	auto p = new ZGIFreeFormPanel(window);

	auto s = new ZGISpinBox(false, 0,0.5f,window);
	auto c = new ZGIComboBox(window);

	s->SetSize({300,100});
	s->SetPosition({600,900});

	c->SetSize({300,100});
	c->SetPosition({300,900});

	for (int i = 0; i < 10; i++)
	{
		auto text = new ZGILabel(window);
		text->SetText("Item " + std::to_string(i) );
		text->SetAutoScaleFont(false);
		text->SetFontSize(100);
		text->SetShouldClipFont(false);
		text->SetShouldWordWrap(false);

		c->AddItem(text);
	}

	p->AddWidget(c);
	p->AddWidget(s);
	//auto image1 = new ZGIImage(window);
	//auto text = new ZGILabel(window);
	//auto spacer = new ZGISpacer(window);

	//auto check = new ZGICheckBox(window);

	//auto progress = new ZGIProgressBar(window);

	//image1->SetImage("test.png");
	//text->SetText("check box");

	//text->SetBoundsFromSize(true);
	//check->SetContent(text);

	//p->AddWidget(image1);
	//p->AddWidget(check);

	window->SetRootContent(p);

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