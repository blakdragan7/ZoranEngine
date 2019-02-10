// ZoransHammer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <filesystem>

#include "ZClassDB.h"

// header library
#include <ThirdParty/cxxopts/cxxopts.hpp>

int main(int argc, char* argv[])
{

	bool shouldRecurse = false;
	std::string directory;
	std::string fileMask;

	cxxopts::Options options("ZoransHammer, Build Reflection Information for c++ classes");

	try
	{

		options.add_options()
			("r,recursive", "If Set, will parse files recursivly in directory", cxxopts::value<bool>()->default_value("false"))
			("d,directory", "Set Directory For Parsing", cxxopts::value<std::string>())
			("m,mask", "The file Mask ex cpp", cxxopts::value<std::string>()->default_value("cpp"))
			("h,help", "Print help");

		auto result = options.parse(argc, argv);

		if (result.count("help") > 0)
		{
			std::cout << options.help({ "" }) << std::endl;
		}
		if (result.count("recursive") > 0)
		{
			shouldRecurse = result["recursive"].as<bool>();
		}
		if (result.count("directory") > 0)
		{
			directory = result["directory"].as<std::string>();
		}
		if (result.count("mask") > 0)
		{
			fileMask = result["mask"].as<std::string>();
		}
	}
	catch (const cxxopts::OptionParseException& e)
	{
		std::cout << "error parsing options: " << e.what() << std::endl;
		std::cout << options.help({ "" });
		return 1;
	}
	catch (cxxopts::OptionSpecException& e)
	{
		std::cout << "Incorrect Argument " << e.what() << std::endl;
		std::cout << options.help({""});
		return 1;
	}
	catch (std::exception& e)
	{
		std::cout << "General Error " << e.what() << std::endl;
		return 1;
	}

	ZClassDB db;
	
	db.ParseSourceFile("TestClass.h");


	db.PrintAllClasses();
	std::cin.get();

	return 0;
}
