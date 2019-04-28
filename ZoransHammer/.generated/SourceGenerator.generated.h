/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class SourceGeneratorClass : public ZClass
{
public:
	SourceGeneratorClass() : ZClass("SourceGenerator", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const SourceGeneratorClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
