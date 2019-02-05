#pragma once

class TestClass
{
private:
	int anInt;
	float aFloat;
	char aChar;
	unsigned int aUInt;

public:
	TestClass();
	~TestClass();

	float GetFloat();
	int GetInt();
	char GetChar();
	unsigned int GetUInt();

	float GetFloat()const;
	int GetInt()const;
	char GetChar()const;
	unsigned int GetUInt()const;

	float GetFloat(int a, int b)const;
	int GetInt(int a, int b)const;
	char GetChar(int a, int b)const;
	unsigned int GetUInt(int a, int b)const;

	static float GetFloat(int a, int b, float c);
	static int GetInt(int a, int b, float c);
	static char GetChar(int a, int b, float c);
	static unsigned int GetUInt(int a, int b, float c);
};

