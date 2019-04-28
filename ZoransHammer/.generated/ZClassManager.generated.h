/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class ZClassManagerClass : public ZClass
{
public:
	ZClassManagerClass() : ZClass("ZClassManager", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const ZClassManagerClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
