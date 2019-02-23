#include "pch.h"
#include "ZClassDB.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <Utils/FileUtils.h>
#include <Utils/StringUtils.h>

#include "SourceParser.h"

void ZClassDB::LoadFromClassDBFile(const char * file)
{
	// currently does nothing
}

ZClassDB::ZClassDB(const char* file)
{
	if (file)
	{
		LoadFromClassDBFile(file);
	}
}

ZClassDB::~ZClassDB()
{
}

void ZClassDB::ParseSourceFile(std::string & file, std::string &dir)
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

ZClass * ZClassDB::ClassForName(const char * name)
{
	auto itr = classMap.find(name);
	if (itr != classMap.end())
		return &itr->second;

	return nullptr;
}

void ZClassDB::InsertClass(ZClass& newClass)
{
	classMap.insert({ newClass.name, newClass });
}

void ZClassDB::PrintAllClasses() const
{
	for (auto c : classMap)
	{
		LOG_ALL << "\nClass Source: " << c.second.sourceFile;
		LOG_ALL << "\n" << c.second.Stringify() << "\n";
	}
}
