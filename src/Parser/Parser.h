#pragma once

#include <fmt/core.h>
#include <fmt/color.h>
#include <string>
#include <tuple>
#include <vector>
#include "./AST/Node.h"

#include "../Utils/TokenDefinitions.h"
#include "../Utils/TypeDefinitions.h"

class Parser
{
public:
	void Parse(std::vector<LexingToken>& lexingTokens);
	static Parser& GetInstance();

private:
	std::vector<LexingToken> _RValueTokenList;
	std::vector<LexingToken> _LValueTokenList;
	
	Parser() = default;

	void Init();

	std::optional<Error> SplitTokenList(std::vector<LexingToken>& lexingTokens);
	std::optional<Error> CheckBrackets(std::vector<LexingToken>& lexingTokens) const;
	
	std::shared_ptr<INode> BuildTree(size_t currentTokenIndex);

};