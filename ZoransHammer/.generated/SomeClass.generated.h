/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class SomeClassClass : public ZClass
{
public:
	SomeClassClass() : ZClass("SomeClass", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const SomeClassClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
