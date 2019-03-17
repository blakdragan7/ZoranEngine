#include "pch.h"
#include "TestClass.h"
#include ".generated/TestClass.generated.cpp"

#include <iostream>

TestClass::TestClass()
{
	f = 1;
}

TestClass::~TestClass()
{
}

float TestClass::GetAFloat()
{
	return 0.0f;
}

double TestClass::GetADouble() const
{
	return 0.0;
}

void TestClass::FunctionWithParams(float x, float y)
{
}

void TestClass::DoAThing()
{
	std::cout << "Did A Thing" << std::endl;
}
