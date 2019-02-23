#include "pch.h"
#include "SourceGenerator.h"

#include <fstream>
#include <direct.h>

#include <iostream>

#include <Utils/StringUtils.h>

#ifndef PATH_MAX
#define PATH_MAX 2046
#endif

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

bool SourceGenerator::GenerateSourceToDir(const ZClass & theClass, std::string dir)
{

	std::string fullPath = dir + "/" + theClass.sourceDir;

	RemoveStringFromString(fullPath, "../");

	LOG_INFO << "Making Dir " << fullPath << std::endl;

	if (int error = mkdir_p(fullPath.c_str()))
	{
		char buff[256] = { 0 };
		strerror_s(buff, error);

		LOG_ERROR << "Error Making DIR " << buff << std::endl;
		return false;
	}

	fullPath += theClass.name + ".generated.h";

	std::fstream file;

	file.open(fullPath, std::ios::out | std::ios::trunc);

	if (file.good())
	{
		std::string classString = theClass.Stringify();
		file.write(classString.c_str(), classString.size());
		file.close();
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
