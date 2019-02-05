#pragma once
#include <fstream>

// moves the cursor to the position directly after theChar or to the end of the file if theChar doesn't exists in the file
// return true if theChar found or false otherwise
extern bool MoveToChar(std::fstream& inFile, char theChar);
// Same as std::getline except it takes a variable number of delimiters
// returns true if found a new line with delim or false if it hasn't
extern bool GetLine(std::fstream& inFile, std::string& line, std::string delims);
