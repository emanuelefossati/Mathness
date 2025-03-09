#pragma once

#include "../Results/Result.h"
#include <unordered_map>

class StorageHandler
{
public:
	static StorageHandler& GetInstance();

	void StoreValue(std::string_view key, EvaluationResult& value);
	EvaluationResult GetValue(std::string_view key) const;

private:
	StorageHandler() = default;

	std::unordered_map<std::string, EvaluationResult> _Storage;
};