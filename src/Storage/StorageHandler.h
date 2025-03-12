#pragma once

#include "../Results/Result.h"
#include <unordered_map>

class StorageHandler
{
public:
	static StorageHandler& GetInstance();

	std::optional<Error> StoreValue(std::string_view key, EvaluationResult& value, std::vector<int> indices = {});
	EvaluationResult GetValue(std::string_view key, std::vector<int> indices = {}) const;

private:
	StorageHandler() = default;

	std::unordered_map<std::string, EvaluationResult> _Storage;
};