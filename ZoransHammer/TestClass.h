#pragma once

#include <vector>
#include <string>

class TestClass
{
private: 
	std::vector<std::string> stringArray; 
	std::string aString;

public:
	TestClass();
	~TestClass();

	static float GetAFloat();
	double GetADouble()const;

	void FunctionWithParams(float x, float y);

};