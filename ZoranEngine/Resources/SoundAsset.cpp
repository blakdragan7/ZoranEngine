#include "stdafx.h"
#include "SoundAsset.h"

#include <Audio/SoundInstance.h>
#include <Audio/AudioEngineBase.h>

#include <Resources/ResourceErrors.h>

#include <Utils/StringAddons.hpp>

#include <iostream>
#include <fstream>

const std::string zsaHeader = "zsa_version_1_0";
const std::string sourceHeader = "zsa_source";
const std::string audioHeader = "zsa_audio";
const std::string typeHeader = "zsa_audio_type";

SoundAsset::SoundAsset() : playableAsset(0), listener(0), isLoaded(false)
{
	sourceFile = new std::string;
	zSourcePath = new std::string;
}

SoundAsset::SoundAsset(AudioListener* listener) : playableAsset(0), listener(listener), isLoaded(false)
{
	sourceFile = new std::string;
	zSourcePath = new std::string;
}

SoundAsset::~SoundAsset()
{
	if (playableAsset)
	{
		aEngine->DestroySound(playableAsset);
	}

	delete sourceFile;
	delete zSourcePath;
}

void SoundAsset::AssignListener(AudioListener * listener)
{
	this->listener = listener;
}

void SoundAsset::Play()
{
	if (listener)
	{
		aEngine->MakeListenerActive(listener);
		aEngine->PlaySoundInstance(playableAsset);
	}
	else
	{
		Log(LogLevel_Error, "Trying to play sound without a listener !");
		return;
	}
}

void SoundAsset::PlayAtLocation(Vec3D location)
{
	if (listener)
	{
		aEngine->MakeListenerActive(listener);
		aEngine->PlaySoundAtLocation(playableAsset,location);
	}
	else
	{
		Log(LogLevel_Error, "Trying to play sound without a listener !");
		return;
	}
}

void SoundAsset::PlayAtLocation(Vec2D location)
{
	if (listener)
	{
		aEngine->MakeListenerActive(listener);
		aEngine->PlaySoundAtLocation(playableAsset, location);
	}
	else
	{
		Log(LogLevel_Error, "Trying to play sound without a listener !");
		return;
	}
}

void SoundAsset::Stop()
{
	if (listener)
	{
		aEngine->MakeListenerActive(listener);
		aEngine->StopSound(playableAsset);
	}
	else
	{
		Log(LogLevel_Error, "Trying to stop sound without a listener !");
		return;
	}
}

void SoundAsset::Play(AudioListener * listener)
{
	aEngine->MakeListenerActive(listener);
	aEngine->PlaySoundInstance(playableAsset);
}

void SoundAsset::PlayAtLocation(Vec3D location, AudioListener * listener)
{
	aEngine->MakeListenerActive(listener);
	aEngine->PlaySoundAtLocation(playableAsset, location);
}

void SoundAsset::PlayAtLocation(Vec2D location, AudioListener * listener)
{
	aEngine->MakeListenerActive(listener);
	aEngine->PlaySoundAtLocation(playableAsset, location);
}

void SoundAsset::Stop(AudioListener * listener)
{
	aEngine->MakeListenerActive(listener);
	aEngine->StopSound(playableAsset);
}

int SoundAsset::MakeFromFile(const std::string & file, AudioListener* listener)
{
	if (isLoaded)
	{
		return RESOURCE_ERROR_ALREADY_LOADED;
	}

	if (listener)
	{
		aEngine->MakeListenerActive(listener);
	}
	else
	{
		if (this->listener)
		{
			aEngine->MakeListenerActive(this->listener);
		}
		else
		{
			aEngine->CreateAudioListener(&this->listener);
		}
	}

	std::string fileExt = GetStringFileType(StringToLower(file));
	AudioFileType fileType;

	if (fileExt == "mp3")
	{
		fileType = AFT_MP3;
	}
	else if (fileExt == "wav")
	{
		fileType = AFT_WAV;
	}
	else
	{
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}

	auto error = aEngine->CreateSoundFromFile(file.c_str(), fileType, &playableAsset);

	if (error != AE_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Creating Sound Asset %s", StringForAudioError(error));
		return RESOURCE_ERROR_LOADING_FILE;
	}

	*sourceFile = file;

	isLoaded = true;

	return RESOURCE_ERROR_NO_ERROR;
}

int SoundAsset::LoadFromFile(const std::string & file, AudioListener* listener)
{
	if (isLoaded == true)
	{
		return RESOURCE_ERROR_ALREADY_LOADED;
	}

	if (listener)
	{
		aEngine->MakeListenerActive(listener);
	}
	else
	{
		if (this->listener)
		{
			aEngine->MakeListenerActive(this->listener);
		}
		else
		{
			aEngine->CreateAudioListener(&this->listener);
		}
	}

	std::string filePath = file;

	if (GetStringFileType(StringToLower(filePath)) != "zsa")
	{
		filePath += ".zsa";
	}

	std::fstream fileS;
	fileS.open(filePath.c_str(), std::ios::binary | std::ios::in);
	
	if (fileS.good())
	{
		std::string line;
		std::getline(fileS, line);

		if (line != zsaHeader)
		{
			Log(LogLevel_Error, "File %s Does not Contain Correct Header !!", filePath.c_str());
			return RESOURCE_ERROR_LOADING_FILE;
		}

		int numHeaders = 0;

		line.clear();
		while (std::getline(fileS, line))
		{
			if (line.empty())continue;

			if (line == sourceHeader)
			{
				std::getline(fileS, *sourceFile);
			}
			else if (line == audioHeader)
			{
				size_t dataSize;
				fileS.read((char*)&dataSize, sizeof(size_t));

				char* data = new char[dataSize];
				fileS.read(data, dataSize);
				{
					std::fstream tmp;
					tmp.open(".~.tmp.wav", std::ios::out | std::ios::trunc | std::ios::binary);
					if (tmp.good())
					{
						tmp.write(data, dataSize);

						tmp.close();
						delete[] data;
					}
					else
					{
						Log(LogLevel_Error, "Sound Asset Could not open tmp file for writing");
						return RESOURCE_ERROR_LOADING_FILE;
					}
				}

				auto err = aEngine->CreateSoundFromFile(".~.tmp.wav", AFT_WAV, &playableAsset);
				if (err != AE_NO_ERROR)
				{
					Log(LogLevel_Error, "Error Creating Audio Asset from %s, error: %s", filePath.c_str(), StringForAudioError(err));
					return RESOURCE_ERROR_LOADING_FILE;
				}
			}

			++numHeaders;
			line.clear();
		}

		if (numHeaders != 2)
		{
			Log(LogLevel_Error, "Incorrect Number of Headers for file %s", filePath.c_str());
			return RESOURCE_ERROR_LOADING_FILE;
		}
	}
	else
	{
		char errorStr[256] = { 0 };
		strerror_s(errorStr, errno);
		Log(LogLevel_Error, "Error opening file %s : %s", file.c_str(), errorStr);
		return RESOURCE_ERROR_LOADING_FILE;
	}

	isLoaded = true;

	fileS.close();

	*zSourcePath = filePath;
	return RESOURCE_ERROR_NO_ERROR;
}

int SoundAsset::SaveToFile(const std::string & file)
{
	if (isLoaded == false)
	{
		return RESOURCE_ERROR_NOT_LOADED;
	}

	std::string filePath = file;

	if (GetStringFileType(StringToLower(filePath)) != "zsa")
	{
		filePath += ".zsa";
	}

	std::string sourceData;

	if (sourceFile->empty() == false)
	{
		std::fstream source;
		source.open(sourceFile->c_str(), std::ios::in | std::ios::binary);
	
		if (source.good())
		{
			char buf[256] = { 0 };
			while (source.eof() == false)
			{
				source.read(buf, 256);
				sourceData.append(buf, source.gcount());
			}
			source.close();
		}
		else
		{
			char errorStr[256] = { 0 };
			strerror_s(errorStr, errno);
			Log(LogLevel_Info, "Source File %s can not be opend {%s} , trying to read from zSourceFile Instead", sourceFile->c_str(), errorStr);

			goto zSourceRead;
		}
	}
	else if(zSourcePath->empty() == false)
	{
	zSourceRead:
		std::fstream file;
		file.open(zSourcePath->c_str(), std::ios::in | std::ios::binary);
		if (file.good())
		{
			std::string line;
			while (std::getline(file, line))
			{
				if (line == audioHeader)
				{
					size_t dataSize = 0;
					file.read((char*)&dataSize, sizeof(size_t));
					char* buff = (char*)malloc(dataSize);
					file.read(buff, dataSize);
					sourceData.append(buff, dataSize);
					free(buff);
				}
				line.clear();
			}
			file.close();
		}
		else
		{
			char errorStr[256] = { 0 };
			strerror_s(errorStr, errno);
			Log(LogLevel_Error, "Error Saving Sound Asset, Source File (%s) and ZSourceFile (%s) Could not be Opened ! %s", sourceFile->c_str(), zSourcePath->c_str(), errorStr);

			return RESOURCE_ERROR_SAVING_FILE;
		}
	}
	else
	{
		Log(LogLevel_Error, "Error Saving Sound Asset, Source File and ZSourceFile are not set !");

		return RESOURCE_ERROR_SAVING_FILE;
	}

	std::fstream fileS;
	fileS.open(filePath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);

	if (fileS.good())
	{
		*zSourcePath = filePath;

		fileS << zsaHeader + "\n";
		fileS << sourceHeader + "\n";
		fileS << *sourceFile + "\n";
		fileS << audioHeader + "\n";

		// write All source into file
		size_t size = sourceData.size();
		fileS.write((char*)&size, sizeof(size_t));
		fileS.write(sourceData.c_str(), sourceData.size());

		fileS.close();
	}
	else
	{
		char errorStr[256] = { 0 };
		strerror_s(errorStr, errno);
		Log(LogLevel_Error, "Error Saving Sound Asset, %s Can't Be Opened for Writing! %s", filePath.c_str(), errorStr);
		return RESOURCE_ERROR_SAVING_FILE;
	}

	return RESOURCE_ERROR_NO_ERROR;
}

int SoundAsset::Save()
{
	if (isLoaded == false)
	{
		return RESOURCE_ERROR_NOT_LOADED;
	}

	if (zSourcePath->empty())
	{
		Log(LogLevel_Error, "Could not re-save file without original");
		return RESOURCE_ERROR_SAVING_FILE;
	}

	return SaveToFile(*zSourcePath);
}

const char * SoundAsset::GetAssetDescription() const
{
	return "SoundAsset - represents a loaded sound asset (like .wav) that can be played";
}
