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

std::optional<EvaluationResult> StorageHandler::GetValue(std::string_view key) const
{
	auto it = _Storage.find(std::string(key));

	if (it == _Storage.end())
	{
		return std::nullopt;
	}

	return it->second;
}