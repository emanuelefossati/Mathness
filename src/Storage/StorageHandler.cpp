#include "StorageHandler.h"

StorageHandler& StorageHandler::GetInstance()
{
	static StorageHandler instance;
	return instance;
}

void StorageHandler::StoreValue(std::string_view key, EvaluationResult& value)
{
	_Storage[std::string(key)] = value;
}

EvaluationResult StorageHandler::GetValue(std::string_view key) const
{
	auto it = _Storage.find(std::string(key));

	if (it == _Storage.end())
	{
		return Error("Identinfier not found");
	}

	return it->second;
}