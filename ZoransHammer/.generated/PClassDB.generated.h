/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class PClassDBClass : public ZClass
{
public:
	PClassDBClass() : ZClass("PClassDB", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const PClassDBClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
