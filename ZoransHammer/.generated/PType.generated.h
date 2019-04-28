/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class PTypeClass : public ZClass
{
public:
	PTypeClass() : ZClass("PType", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const PTypeClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
