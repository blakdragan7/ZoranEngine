#pragma once

#include <vector>
#include <string>

#include "BaseTestClass.h"

class TestClass : public BaseTestClass
{
private: 
	std::vector<std::string> stringArray; 
	std::string aString;

public:
	TestClass(); // this is a comment
	~TestClass();
	
	static float GetAFloat();
	double GetADouble()const;

	void FunctionWithParams(float x, float y);

};