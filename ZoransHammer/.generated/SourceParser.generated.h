/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class SourceParserClass : public ZClass
{
public:
	SourceParserClass() : ZClass("SourceParser", {}) {}
	void* SpawnDynamic()const override;
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const SourceParserClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
