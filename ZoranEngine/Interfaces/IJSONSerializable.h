#pragma once

#include <ThirdParty/nlohmann/json.hpp>

class IJSONSerializable
{
public:
	virtual ~IJSONSerializable() {}

	virtual nlohmann::json Serialize()const = 0;
	virtual bool FromJSON(const nlohmann::json&) = 0;

	virtual void operator=(const nlohmann::json& rh) { FromJSON(rh); }
	virtual  operator nlohmann::json()const { return Serialize(); }
};

void to_json(nlohmann::json& j, const IJSONSerializable& p) {
	j = p.Serialize();
}

void from_json(const nlohmann::json& j, IJSONSerializable& p) {
	p.FromJSON(j);
}
