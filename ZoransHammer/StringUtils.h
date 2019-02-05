#pragma once
#include <string>
#include <vector>
// removes all of toRmove (each char individually) from string
extern void FlattenString(std::string& string, std::string toRemove = " \t\n");
// returns position of first char from toFind found in string
extern size_t FindAny(std::string& string, std::string toFind);
// splits the string using any char in delims at the delim
extern std::vector<std::string>Split(std::string& string, std::string delims);