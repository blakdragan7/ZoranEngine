/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class TestClassClass : public ZClass
{
public:
	TestClassClass() : ZClass("TestClass", {"BaseTestClass"}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const TestClassClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
