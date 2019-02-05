// ZoransHammer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "ZClassDB.h"

int main()
{
	ZClassDB db;
	db.ParseSourceFile("TestClass.h");
	db.PrintAllClasses();
}
