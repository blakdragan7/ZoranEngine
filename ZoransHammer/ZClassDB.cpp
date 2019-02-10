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

std::string modifiers[] =
{
	"const",
	"static"
};

bool IsModifier(std::string s)
{
	for (auto l : modifiers)
		if (s == l)return true;
	return false;
}

void ZClassDB::LoadFromClassDBFile(const char * file)
{
	// currently does nothing
}

ZType ZClassDB::ParseType(std::string& info)
{
	auto splitStr = SplitString(info, " "); // split string by whitespace

	ZType type;

	type.isStatic = splitStr[0] == "static";

	type.memberName = splitStr.back();
	splitStr.pop_back();

	for (auto s : splitStr)
	{
		if (s.empty())continue;
		
		if (type.typeName.empty() == false)
			type.typeName += " ";
		type.typeName += s;
	}

	return type;
}

ZFunction ZClassDB::ParseFunction(std::string& info)
{

	std::string typeInfo;

	auto fStrings = SplitString(info, "(");

	ZFunction function;

	auto _Fstrings = SplitString(fStrings[0], " ");

	function.name = _Fstrings.back();

	if (_Fstrings.size() > 1)
	{
		_Fstrings.pop_back();

		if (_Fstrings[0] == "static")
		{
			function.isStatic = true;
			_Fstrings.erase(_Fstrings.begin());
		}

		if (_Fstrings[0] == "virtual")
		{
			function.isVirtual = true;
			_Fstrings.erase(_Fstrings.begin());
		}

		for (auto s : _Fstrings)
		{
			if (IsModifier(s))continue;
			else
			{
				function.returnType.typeName += s;
			}
		}

		function.returnType.memberName = "function_return";
	}

	 auto _vs = SplitString(fStrings[1], ")");
	
	 FlattenString(_vs[1], " ");

	 bool isConst = _vs[1] == "const";

	if (_vs[0].empty())
	{
		// function has no params
	}
	else
	{
		// function does have params
		auto _Pstrings = SplitString(_vs[0], ",");

		for (auto s : _Pstrings)
		{
			if (s.empty())continue;
			ZType type = ParseType(s);
			function.parameters.push_back(type);
		}
	}

	return function;
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

void ZClassDB::ParseSourceFile(const char * file)
{
	using namespace std;
	fstream inFile;
	inFile.open(file, ios::in);

	if (inFile.good() == false)
	{
		cout << "Could not open File !\n";
		return;
	}

	ZClass theClass;
	vector<string> members;
	vector<string> functions;

	while (inFile.eof() == false)
	{
		string word;
		inFile >> word;
		if (word == "class")
		{
			inFile >> theClass.name;
			
			if (MoveToChar(inFile, '{') == false)
			{
				cout << "Malformed Class File !" << endl;
				inFile.close();
				return;
			}

			std::string line;
			while (GetLineVar(inFile, line, { ": ", ";", ":\n", ":\t"}))
			{
				FlattenString(line, "\n\t");
				if (line.empty())continue;
				if (line == "private" || line == "public" || line == "protected")continue;
				if (line.find("}") != string::npos)break;
				if (line.find("(") != string::npos)
				{
					ZFunction function = ParseFunction(line);
					theClass.functions.push_back(function);
				}
				else
				{
					ZType type = ParseType(line);
					theClass.members.push_back(type);
				}
			}
		}
	}

	classMap.insert({ theClass.name, theClass });

	inFile.close();
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
		std::cout << "Begin Class\n";
		std::cout << c.second.Stringify();
		std::cout << "\nEnd Class\n";
	}
}
