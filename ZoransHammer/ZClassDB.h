#pragma once
#include <map>
#include "ZClass.h"
#include "ZType.h"
#include "ZFunction.h"
// This is a database of classes with the name of the class as the key.

class ZClassDB
{
private:
	std::map<std::string, ZClass> classMap;

private:
	void LoadFromClassDBFile(const char* file);

public:
	// file is a database file of previously loaded class definitions
	// if null or invalid it will assume there was no previously loaded class information
	ZClassDB(const char* file = 0);
	~ZClassDB();

	// Parse the source file and add the class to the database or update the class if it's already added
	void ParseSourceFile(std::string &file, std::string & dir);

	ZClass* ClassForName(const char* name);
	void InsertClass(ZClass& newClass);

	void PrintAllClasses()const;

	// iteration functions

	inline std::map<std::string, ZClass>::iterator begin() { return classMap.begin(); }
	inline std::map<std::string, ZClass>::iterator end() { return classMap.end(); }
};

