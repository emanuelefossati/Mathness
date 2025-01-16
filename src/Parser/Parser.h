#pragma once

#include <fmt/core.h>
#include <fmt/color.h>
#include <string>
#include <tuple>
#include <vector>
#include "./AST/Node.h"
#include "./AST/Operation Nodes/Binary/BinaryOperations.h"
#include "./AST/Operation Nodes/Unary/UnaryOperations.h"
#include "./AST/Value Nodes/ValueNodes.h"

#include "../Utils/TokenDefinitions.h"
#include "../Utils/TypeDefinitions.h"

using NodeResult = std::variant<std::shared_ptr<INode>, Error>;

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
	
	NodeResult BuildTree(size_t currentTokenIndex);

};