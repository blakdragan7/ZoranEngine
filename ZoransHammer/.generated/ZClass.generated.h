/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class ZClassClass : public ZClass
{
public:
	ZClassClass() : ZClass("ZClass", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const ZClassClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
