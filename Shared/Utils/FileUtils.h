#pragma once
#include <fstream>
#include <vector>

#include <functional>

#include "PlatoformTypes.h"

// moves the cursor to the position directly after theChar or to the end of the file if theChar doesn't exists in the file
// return true if theChar found or false otherwise
DLL_EXPORT bool MoveToChar(std::fstream& inFile, char theChar);
// Same as std::getline except it takes a variable number of delimiters
// returns true if found a new line with delim or false if it hasn't
DLL_EXPORT bool GetLine(std::fstream& inFile, std::string& line, std::string delims);
// Same as GetLine except it searches for the entire string for each string in delims and returns when one is found
// returns true if found a new line with delim or false if it hasn't
DLL_EXPORT bool GetLineVar(std::fstream& inFile, std::string& line, std::vector<std::string> delims);
// Passes each file in inDir to callback as it finds them. if Recurse is true it will recursivle go through every sub directory
// fileMask is the mask for the file to get using * as a wild card so *.* means everything *.h is only .h files etc..
// return true if inDir is found and can be opened and false otherwise
// the callback decl is this void Callback(std::string fileName, std::string filePath)
DLL_EXPORT int GetFilesInDir(std::string inDir, std::string fileMask,bool recurse, std::function<void(std::string, std::string)> callback);