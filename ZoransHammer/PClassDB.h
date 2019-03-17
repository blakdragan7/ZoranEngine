#pragma once
#include <map>
#include "PClass.h"
#include "PType.h"
#include "PFunction.h"
// This is a database of classes with the name of the class as the key.

class PClassDB
{
private:
	std::map<std::string, PClass> classMap;

private:
	void LoadFromClassDBFile(const char* file);

public:
	// file is a database file of previously loaded class definitions
	// if null or invalid it will assume there was no previously loaded class information
	PClassDB(const char* file = 0);
	~PClassDB();

	// Parse the source file and add the class to the database or update the class if it's already added
	void ParseSourceFile(std::string &file, std::string & dir);

	PClass* ClassForName(const char* name);
	void InsertClass(PClass& newClass);

	void PrintAllClasses()const;

	// iteration functions

	inline std::map<std::string, PClass>::iterator begin() { return classMap.begin(); }
	inline std::map<std::string, PClass>::iterator end() { return classMap.end(); }
};

