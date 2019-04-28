/* Generated For Reflection Information With Zoran'shammer */
#pragma once

#include <ZClass.h>

class BaseTestClassClass : public ZClass
{
public:
	BaseTestClassClass() : ZClass("BaseTestClass", {}) {}
};
#undef GENERATED_ZCLASS
#define GENERATED_ZCLASS static const BaseTestClassClass Class; \
static const ZClass* GetStaticClass() {return &Class;}
