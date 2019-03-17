#include "pch.h"
#include "SourceParser.h"

#include <Utils/StringUtils.h>
#include <Utils/FileUtils.h>

#include <algorithm>

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

void SourceParser::RemoveEmptyStrings(std::vector<std::string>& vector)
{
	vector.erase(std::remove_if(vector.begin(), vector.end(), [](std::string& s) {return s.empty(); }), vector.end());
}

std::vector<std::string> SourceParser::SplitAtFirst(std::string & string, const char delim)
{
	std::vector<std::string> ret;

	std::string first;
	bool firstB = true;

	for (auto c : string)
	{
		if (c == delim && firstB)
		{
			ret.push_back(first);
			first.clear();
			firstB = false;
		}
		else
			first.push_back(c);
	}

	ret.push_back(first);

	return ret;
}

bool SourceParser::SkipTemplate(std::fstream & inFile)
{
	char test[3] = { 0 };

	bool isInComment = false;
	char commentType = 0;

	int depth = 0;

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
				LOG_ERROR << "SourceParser Incorrect Comment State for GetLine\n";
				break;
			}

			continue;
		}
		else
		{
			if (test[1] == '{')
			{
				++depth;
				test[0] = 0;
				test[1] = 0;
				continue;
			}
			else if (test[1] == '}')
			{
				--depth;
				if (depth < 1)
					return true;
				continue;
			}
			else if (test[1] == ';')
			{
				// this is a template class declaration
				return true;
			}
			else if (strcmp(test, "//") == 0) // start comment with type /
			{
				isInComment = true;
				commentType = '/';
				test[0] = 0;
				test[1] = 0;
				continue;
			}
			else if (strcmp(test, "/*") == 0) // start comment with type *
			{
				isInComment = true;
				commentType = '*';
				test[0] = 0;
				test[1] = 0;
				continue;
			}
		}
	}

	return false;
}

bool SourceParser::MoveOutOfImplementation(std::fstream & inFile, int & depth)
{
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
				LOG_ERROR << "SourceParser Incorrect Comment State for GetLine\n";
				break;
			}

			continue;
		}
		else
		{
			if (test[1] == '{')
			{
				++depth;
				test[0] = 0;
				test[1] = 0;
				continue;
			}
			else if (test[1] == '}')
			{
				--depth;
				if (depth < 1)
					return true;
				continue;
			}
			else if (strcmp(test, "//") == 0) // start comment with type /
			{
				isInComment = true;
				commentType = '/';
				test[0] = 0;
				test[1] = 0;
				continue;
			}
			else if (strcmp(test, "/*") == 0) // start comment with type *
			{
				isInComment = true;
				commentType = '*';
				test[0] = 0;
				test[1] = 0;
				continue;
			}
		}
	}

	return false;
}

int SourceParser::GoToEndOfClassDec(std::fstream& inFile, std::string & line)
{
	while (inFile.eof() == false)
	{
		char test[2] = { 0 };
		test[0] = inFile.get();
		if (test[0] == ';')
		{
			return 2;
		}
		else if (test[0] == '{')
		{
			return 1;
		}
		else
		{
			line.append(1, test[0]);
			continue;
		}
	}

	return 0;
}

PType SourceParser::ParseType(std::string & info)
{
	auto splitStr = SplitString(info, " "); // split string by whitespace

	PType type;

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

bool SourceParser::ParseFunction(std::string & info, PFunction& function)
{
	std::string typeInfo;

	auto fStrings = SplitAtFirst(info, '(');

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

	if (_vs.size() != 2)
	{
		LOG_ERROR << "Malformed Function Info " << info << std::endl;
		return false;
	}

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
			PType type = ParseType(s);
			function.parameters.push_back(type);
		}
	}
	if (_vs[1] == "=0")
	{
		function.isAbstract = true;
		function.isVirtual = false;
	}

	return true;
}

int SourceParser::ParseClassDeclaration(PClass & theClass, std::string info)
{
	using namespace std;

	string copy = info;
	FlattenString(copy, " \n\t");
	if (copy.empty())return 2;

	vector<std::string> cDefs = SplitString(info, ":");

	RemoveEmptyStrings(cDefs);

	if (cDefs.size() == 2) // has a parent class
	{
		string classString = cDefs[0];
		auto words = SplitString(classString, " \t");
		RemoveEmptyStrings(words);
		theClass.name = words.back();

		auto parents = SplitString(cDefs[1], ",");
		for (auto parentString : parents)
		{
			auto parts = SplitString(parentString, " \t");
			RemoveEmptyStrings(parts);
			theClass.parents.push_back(parts.back());
		}
	}
	else if (cDefs.size() != 1)
	{
		return 1;
	}
	else
	{
		FlattenString(info, "\n");
		auto words = SplitString(info, " \t");
		RemoveEmptyStrings(words);
		theClass.name = words.back();
	}

	return 0;
}

bool SourceParser::GetLine(std::fstream & inFile, std::string & line)
{
	size_t maxStringSize = 2;

	line.clear();
	
	char test[3] = { 0 };

	bool isInImplementation = false;
	bool isInComment = false;
	char commentType = 0;

	while (inFile.eof() == false)
	{
		test[0] = test[1]; // move second position back one

		char g = inFile.get();
		test[1] = g;

		if(g == '\n')currentLine++;

		if (isInImplementation)
		{
			if (g == '}')
			{
				isInImplementation = false;
				return true;
			}

			continue;
		}

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
				LOG_ERROR << "SourceParser Incorrect Comment State for GetLine\n";
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
			else if (test[1] == '{')
			{
				isInImplementation = true;
				test[0] = 0;
				test[1] = 0;
				continue;
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

bool SourceParser::ParseFile(std::string& file, std::string &dir)
{
	using namespace std;
	currentLine = 0;
	fstream inFile;
	inFile.open(file, ios::in);

	if (inFile.good() == false)
	{
		LOG_ERROR << "Could not open File !\n";
		return false;
	}

	/*std::string dir;
#ifdef _WIN32
	char sep = '\\';
#else
	char sep = '/';
#endif
	size_t l = file.rfind(sep, dir.size());
	if (l != string::npos)
		dir = file.substr(0, l);
	else
		dir = "./";*/

	while (inFile.eof() == false)
	{
		string word;
		inFile >> word;
		if (word == "//") // is this a comment
		{
			MoveToChar(inFile, '\n');// skip line
			continue;
		}
		else if (word == "/*")
		{
			bool found = false;
			char test[3] = { 0 };
			while (inFile.eof() == false)
			{
				test[0] = test[1];
				test[1] = inFile.get();

				if (strcmp(test, "*/") == 0)
				{
					// we found end of comment
					found = true;
					break;
				}
			}

			if (found == false)
			{
				LOG_ERROR << "Malformed Header file ! Could not Move Out Of /* comment !\n";
				inFile.close();
				return false;
			}

			continue;
		}
		else if (word == "extern")
		{
			inFile >> word;
			if (word.find("\n") != string::npos)currentLine++;
			if (word == "\"C\"")
			{
				// we are in c code

				int depth = 0;
				if (MoveToChar(inFile, '{'))
				{
					depth = 1;

					if (MoveOutOfImplementation(inFile, depth) == false)
					{
						LOG_ERROR << "Malformed Header file ! Could not Move Out Of Extern C !\n";
						inFile.close();
						return false;
					}

					continue;
				}
				else
				{
					LOG_ERROR << "Malformed Header file ! Extern C without {\n";
					inFile.close();
					return false;
				}
			}
		}
		else if (word == "union")
		{
			int depth = 0;
			if (MoveToChar(inFile, '{'))
			{
				depth = 1;

				if (MoveOutOfImplementation(inFile, depth) == false)
				{
					LOG_ERROR << "Malformed Header file ! Could not Move Out Of union !\n";
					inFile.close();
					return false;
				}

				continue;
			}
		}
		else if (word == "class" || word == "struct")
		{
			bool isClass = word == "class";
			PClass theClass;

			theClass.sourceFile = file;
			theClass.sourceDir = dir;

			std::string line;
			int res = GoToEndOfClassDec(inFile, line);
			if (res == 0)
			{
				LOG_ERROR << "Malformed Class File " << file << " could not find end of class decl" << endl;
				inFile.close();
				return false;
			}
			if (res == 2)
			{
				continue;
			}

			if (int err = ParseClassDeclaration(theClass, line))
			{
				switch (err)
				{
				case 1:
					LOG_ERROR << "Malformed Class File " << file << " incorrect : count" << endl;
					break;
				case 2:
					// ignore because no declaration
					continue;
					break;
				default:
					LOG_ERROR << "Malformed Class File " << file << " Unkown ParseClassDeclaration error" << endl;
					break;
				}
				inFile.close();
				return false;
			}

			line.clear();

			while (GetLine(inFile, line))
			{
				FlattenString(line, "\n\t");
				if (line.empty())continue;
				if (line == "private" || line == "public" || line == "protected")continue;
				if (line.find("}") != string::npos)break;
				if (line.find("(") != string::npos)
				{
					PFunction function;
					if (ParseFunction(line, function))
					{
						theClass.functions.push_back(function);
						if (function.isAbstract)
						{
							theClass.isInterface = true;
						}
					}
					else
						return false; // try to avoid parsing unwanted files
				}
				else
				{
					PType type = ParseType(line);
					theClass.members.push_back(type);
				}
			}

			generated.push_back(theClass);
		}
		else if (word.find("template") != string::npos)
		{
			if(SkipTemplate(inFile) == false)
			{
				LOG_ERROR << "Malformed Class File " << file << " could not skip template" << endl;
				inFile.close();
				return false;
			}

			continue;
		}
	}

	inFile.close();

	return true;
}
