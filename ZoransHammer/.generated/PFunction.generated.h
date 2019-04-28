/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class PFunctionClass : public ZClass
{
public:
	PFunctionClass() : ZClass("PFunction", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const PFunctionClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
