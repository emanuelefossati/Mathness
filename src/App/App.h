#pragma once
#include "./Lexer/Lexer.h"
#include "./Parser/Parser.h"
#include "./Storage/StorageHandler.h"

#include <fmt/core.h>
#include <fmt/color.h>

class App
{
public:
	void Run();
	static App& GetInstance();

private:
	App() = default;

	void ShowResult(EvaluationResult& result) const;
	static std::optional<Error> HandleAssignment(std::vector<LexingToken>& leftExpressionTokenList, EvaluationResult& rightExpressionResult);
	static void PrintError(Error error);
	static void ClearComments(std::string& input);
};

