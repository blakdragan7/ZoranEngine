#include "pch.h"
#include "SourceGenerator.h"

#include <fstream>
#include <direct.h>

#include <iostream>

#include <Utils/StringUtils.h>

#ifndef PATH_MAX
#define PATH_MAX 2046
#endif

const char* SourceHeader_Header =
"/* Generated For Reflection Information With Zoran's Hammer */\n#pragma once\n\n";


const char* SourceSource_Header =
"/* Generated For Reflection Information With Zoran'shammer */\n\n";

int mkdir_p(const char *path)
{
	/* Adapted from http://stackoverflow.com/a/2336245/119527 */
	/* Modified again to use strcpy_c and to return error codes instead of setting errno*/
	const size_t len = strlen(path);
	char _path[PATH_MAX];
	char *p;

	errno = 0;

	/* Copy string so its mutable */
	if (len > sizeof(_path) - 1) {
		return ENAMETOOLONG;
	}
	strcpy_s(_path, path);

	/* Iterate the string */
	for (p = _path + 1; *p; p++) {
		if (*p == '/') {
			/* Temporarily truncate */
			*p = '\0';

			if (_mkdir(_path) != 0) {
				if (errno != EEXIST)
					return errno;
			}

			*p = '/';
		}
	}

	if (_mkdir(_path) != 0) {
		if (errno != EEXIST)
			return errno;
	}

	return 0;
}

bool SourceGenerator::GenerateSourceToDir(const PClass & theClass, std::string headerDir)
{
	std::string fullPath = headerDir;

	LOG_INFO << "Making Dir " << fullPath << std::endl;

	if (int error = mkdir_p(fullPath.c_str()))
	{
		char buff[256] = { 0 };
		strerror_s(buff, error);

		LOG_ERROR << "Error Making DIR " << buff << std::endl;
		return false;
	}

	std::string headerPath;

	if(fullPath.back() == '/')
		headerPath = fullPath + theClass.name + ".generated.h";
	else
		headerPath = fullPath + "/" + theClass.name + ".generated.h";

	std::fstream file;

	LOG_INFO << "Generating Header" << std::endl;

	file.open(headerPath, std::ios::out | std::ios::trunc);

	std::string GenClassName = theClass.name + "Class";

	if (file.good())
	{
		file << SourceHeader_Header;

		file << "#include <Reflection/ZClass.h>\n\n";
		file << "class " << GenClassName << " : public ZClass\n";

		file << "{\n";

		file << "public:\n";

		file << "\t" << GenClassName << "() : ZClass(\"" << theClass.name << "\", {";

		if (theClass.parents.size())
		{
			for (int i=0;i<theClass.parents.size();i++)
			{
				std::string parent = theClass.parents[i];
				FlattenString(parent);
				auto c = parent;
				file << "\"" << c << "\"";
				if (i != theClass.parents.size() - 1)
					file << ", ";
			}
		}

		file << "}) {}\n";
		if(theClass.isInterface == false)
			file << "\tvoid* " << "SpawnDynamic()const override;\n";
		file << "};\n";


		file << "#undef GENERATED_ZCLASS\n";
		file << "#define GENERATED_ZCLASS ";
		file << "static const " << GenClassName << " i" << GenClassName << "; \\\n";
		file << "static const ZClass* GetStaticClass() {return &i"<< GenClassName << ";}\n";

		file.close();

		/*"\t{ \n\t\treturn(void*)(new " << theClass.name << "); \n\t }\n";
		file << "#include \"" << theClass.sourceFile << "\"\n\n";

		*/

	}
	else
	{
		char errorS[256] = { 0 };
		strerror_s(errorS, errno);
		LOG_ERROR << "Error Opening File For Ouput " << fullPath << " error: " << errorS << std::endl;
		return false;
	}

	LOG_INFO << "Generating Source" << std::endl;

	std::string sourcePath;

	if (fullPath.back() == '/')
		sourcePath = fullPath + theClass.name + ".generated.cpp";
	else
		sourcePath = fullPath + "/" + theClass.name + ".generated.cpp";

	file.open(sourcePath, std::ios::out | std::ios::trunc);

	if (file.good())
	{
		file << SourceSource_Header;

		//file << "#include \"" << (theClass.name + ".generated.h") << "\"\n\n";
		//file << "#include \"" << theClass.sourceFile << "\"\n\n";

		file << "const " << GenClassName << " " << theClass.name << "::i" << GenClassName << ";\n";
		if (theClass.isInterface == false)
		{
			file << "void* " << GenClassName << "::" << "SpawnDynamic()const\n";
			if (theClass.isInterface == false)
				file << "{\n\treturn (void*)(new " << theClass.name << ");\n}\n";
			else
				file << "{\n\treturn 0;\n}\n";
		}

		file.close();

		/*"\t{ \n\t\treturn(void*)(new " << theClass.name << "); \n\t }\n";
		file << "#include \"" << theClass.sourceFile << "\"\n\n";

		*/

	}
	else
	{
		char errorS[256] = { 0 };
		strerror_s(errorS, errno);
		LOG_ERROR << "Error Opening File For Ouput " << fullPath << " error: " << errorS << std::endl;
		return false;
	}

	return true;
}
