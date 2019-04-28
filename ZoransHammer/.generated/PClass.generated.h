/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class PClassClass : public ZClass
{
public:
	PClassClass() : ZClass("PClass", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const PClassClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
