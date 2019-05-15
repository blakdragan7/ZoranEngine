#pragma once

#include ".generated/BaseTestClass.generated.h"

class BaseTestClass
{
protected:
	int f;

public:
	// doesn't work with abstract classes
	// needs to not implement void* BaseTestClassClass::SpawnDynamic()const for abstract classes
	GENERATED_ZCLASS;

	BaseTestClass();
	virtual void DoAThing() = 0;
	virtual void DoAnotherThing();

};

