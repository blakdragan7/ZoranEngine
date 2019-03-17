#include "pch.h"
#include "PClassDB.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <Utils/FileUtils.h>
#include <Utils/StringUtils.h>

#include "SourceParser.h"

void PClassDB::LoadFromClassDBFile(const char * file)
{
	// currently does nothing
}

PClassDB::PClassDB(const char* file)
{
	if (file)
	{
		LoadFromClassDBFile(file);
	}
}

PClassDB::~PClassDB()
{
}

void PClassDB::ParseSourceFile(std::string & file, std::string &dir)
{
	SourceParser parser;

	if (parser.ParseFile(file, dir))
	{
		auto classArray = parser.GetGeneratedClass();

		for (auto& c : classArray)
		{
			classMap.insert({ c.name,c });
		}	
	}
}

PClass * PClassDB::ClassForName(const char * name)
{
	auto itr = classMap.find(name);
	if (itr != classMap.end())
		return &itr->second;

	return nullptr;
}

void PClassDB::InsertClass(PClass& newClass)
{
	classMap.insert({ newClass.name, newClass });
}

void PClassDB::PrintAllClasses() const
{
	for (auto c : classMap)
	{
		LOG_ALL << "\nClass Source: " << c.second.sourceFile;
		LOG_ALL << "\n" << c.second.Stringify() << "\n";
	}
}
