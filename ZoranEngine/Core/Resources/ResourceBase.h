#pragma once
/*
*	Abstraction of resource files that have been loaded from disk.
*/

class ZoranEngine_EXPORT ResourceBase
{
protected:
	bool isLoaded;

public:
	ResourceBase();
	virtual ~ResourceBase();

	virtual int LoadFromFile(const std::string& file) = 0;
	virtual int SaveToFile(const std::string& file)=0;
	virtual void DestroyResource() = 0;

	virtual const char* GetResourceDescription()const = 0;

	inline bool GetIsLoaded() { return isLoaded; }
};

