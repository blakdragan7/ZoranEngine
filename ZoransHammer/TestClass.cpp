#include "pch.h"
#include "TestClass.h"


TestClass::TestClass() : anInt(0), aFloat(0), aChar(0), aUInt(0)
{
}


TestClass::~TestClass()
{
}

float TestClass::GetFloat()
{
	return aFloat;
}

int TestClass::GetInt()
{
	return anInt;
}

char TestClass::GetChar()
{
	return aChar;
}

unsigned int TestClass::GetUInt()
{
	return aUInt;
}
