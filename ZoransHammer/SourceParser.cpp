#include "pch.h"
#include "SourceParser.h"

#include <Utils/StringUtils.h>
#include <Utils/FileUtils.h>

#include <iostream>

const std::string modifiers[] =
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

ZType SourceParser::ParseType(std::string & info)
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

ZFunction SourceParser::ParseFunction(std::string & info)
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

bool SourceParser::GetLine(std::fstream & inFile, std::string & line)
{
	size_t maxStringSize = 2;

	line.clear();
	
	char test[3] = { 0 };

	bool isInComment = false;
	char commentType = 0;

	while (inFile.eof() == false)
	{
		test[0] = test[1]; // move second position back one

		char g = inFile.get();
		test[1] = g;

		// list of if chains for each delim
		// faster then using std::string
		if (isInComment)
		{
			switch (commentType)
			{
			case '/':
				// '//' comments only end at newline which is standardized by fstream to \n
				if (test[1] == '\n')
				{
					isInComment = false;
					commentType = 0;
					test[0] = 0;
					test[1] = 0;
				}
				break;
			case '*':
				// '/*' comments are only ended with */ charecters
				if (strcmp(test, "*/") == 0)
				{
					isInComment = false;
					commentType = 0;
					test[0] = 0;
					test[1] = 0;
				}
				break;
			default:
				std::cout << "SourceParser Incorrect Comment State for GetLine\n";
				break;
			}

			continue;
		}
		else
		{
			if (test[1] == ';')
			{
				return true;
			}
			else if (strcmp(test, ": ") == 0)
			{
				line.pop_back(); // just remove last one since we know max size is 2
				return true;
			}
			else if (strcmp(test, ":\n") == 0)
			{
				line.pop_back(); // just remove last one since we know max size is 2
				return true;
			}
			else if (strcmp(test, ":\r") == 0)
			{
				line.pop_back(); // just remove last one since we know max size is 2
				return true;
			}
			else if (strcmp(test, "//") == 0) // start comment with type /
			{
				line.pop_back(); // just remove last one since we know max size is 2
				isInComment = true;
				commentType = '/';
				test[0] = 0;
				test[1] = 0;
				continue;
			}
			else if (strcmp(test, "/*") == 0) // start comment with type *
			{
				line.pop_back(); // just remove last one since we know max size is 2
				isInComment = true;
				commentType = '*';
				test[0] = 0;
				test[1] = 0;
				continue;
			}

			line.append(1, g);
		}
	}

	return false;
}

SourceParser::SourceParser()
{
}

SourceParser::~SourceParser()
{
}

bool SourceParser::ParseFile(std::string file)
{
	using namespace std;
	fstream inFile;
	inFile.open(file, ios::in);

	if (inFile.good() == false)
	{
		cout << "Could not open File !\n";
		return false;
	}

	while (inFile.eof() == false)
	{
		string word;
		inFile >> word;
		if (word == "struct")
		{
			cout << "No Struct support currently\n";
			return false;
		}
		if (word == "//") // is this a comment
		{
			MoveToChar(inFile, '\n');// skip line
			continue;
		}
		if (word == "class")
		{
			ZClass theClass;

			inFile >> theClass.name;

			if (MoveToChar(inFile, '{') == false)
			{
				cout << "Malformed Class File " << file << " !" << endl;
				inFile.close();
				return false;
			}

			std::string line;
			while (GetLine(inFile, line))
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

			generated.push_back(theClass);
		}
	}

	inFile.close();

	return true;
}
