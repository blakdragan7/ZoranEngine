#pragma once

#include <string>
#include <vector>

#include "PlatoformTypes.h"

// Return true if fullString ends with exactly ending or false otherwise
DLL_EXPORT bool StringEndsWithString(std::string const &fullString, std::string const &ending);
// Converts the string to lower case
DLL_EXPORT std::string StringToLower(const std::string & source);
// Returns the string extension if it's the name of a file (ex txt in file.txt)
DLL_EXPORT std::string GetStringFileType(const std::string & filePath);
// removes all of toRmove (each char individually) from string
DLL_EXPORT void FlattenString(std::string& string, const std::string toRemove = " \t\n");
// returns position of first char from toFind found in string
DLL_EXPORT size_t FindAny(const std::string& string, const std::string toFind);
// splits the string using any char in delims at the delim
DLL_EXPORT std::vector<std::string> SplitString(const std::string& string, const std::string delims);