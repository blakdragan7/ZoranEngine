// ZoransHammer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <filesystem>

#include "PClassDB.h"
#include "SourceGenerator.h"

#include <Utils/FileUtils.h>

// header library
#include <ThirdParty/cxxopts/cxxopts.hpp>

#include <Utils/FileLogger.h>
#include <Utils/ConsoleLogger.h>

LoggerBase* logger = 0;

#include "TestClass.h"

int main(int argc, char* argv[])
{
	
	//logger = new FileLogger("error.log");
	logger = new ConsoleLogger();

	bool shouldRecurse = false;
	std::string directory;
	std::string fileMask = ".h";

	cxxopts::Options options("ZoransHammer, Build Reflection Information for c++ classes");

	try
	{
		options.add_options()
			("r,recursive", "If Set, will parse files recursivly in directory", cxxopts::value<bool>()->default_value("false"))
			("d,directory", "Set Directory For Parsing, This is required.", cxxopts::value<std::string>())
			("m,mask", "The file Mask ex cpp", cxxopts::value<std::string>()->default_value(".h"))
			("h,help", "Print help");

		auto result = options.parse(argc, argv);

		if (result.count("help") > 0)
		{
			LOG_ALL << options.help({ "" }) << std::endl;
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
		LOG_ALL << "error parsing Arguments: " << e.what() << std::endl;
		LOG_ALL << options.help({ "" });
		return 1;
	}
	catch (cxxopts::OptionSpecException& e)
	{
		LOG_ALL << "Incorrect Arguments " << e.what() << std::endl;
		LOG_ALL << options.help({""});
		return 1;
	}
	catch (std::exception& e)
	{
		LOG_ALL << "General Error " << e.what() << std::endl;
		return 1;
	}

	if (directory.empty())
	{
		LOG_ALL << "Directory Option Required ! try -h | --help for more information\n";
		return 0;
	}

	PClassDB db;
	
	std::string GeneratedSourceDir = ".generated";

	int error = GetFilesInDir(directory, fileMask, shouldRecurse, [&db, GeneratedSourceDir](std::string filename, std::string filepath) {
		if (filename == "pch.h")return;
		if (filename == "stdafx.h")return;
		if (filename.find(GeneratedSourceDir) != std::string::npos)return;
		std::string fullPath = filepath + filename;
		db.ParseSourceFile(fullPath, filepath);
	});

	if (error)
	{
		LOG_ERROR << "Error Opening Directory " << directory << " with error " << error << std::endl;
	}
	else
	{
		for (auto& c : db)
		{
			if (SourceGenerator::GenerateSourceToDir(c.second, GeneratedSourceDir) == false)
			{
				LOG_ERROR << "Failed Generating Source For " << c.second.name << std::endl;
			}
		}
	}
	return 0;
}
