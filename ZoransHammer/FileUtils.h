#pragma once
#include <fstream>
#include <vector>

// moves the cursor to the position directly after theChar or to the end of the file if theChar doesn't exists in the file
// return true if theChar found or false otherwise
extern bool MoveToChar(std::fstream& inFile, char theChar);
// Same as std::getline except it takes a variable number of delimiters
// returns true if found a new line with delim or false if it hasn't
extern bool GetLine(std::fstream& inFile, std::string& line, std::string delims);
// Same as GetLine except it searches for the entire string for each string in delims and returns when one is found
// returns true if found a new line with delim or false if it hasn't
extern bool GetLineVar(std::fstream& inFile, std::string& line, std::vector<std::string> delims);
