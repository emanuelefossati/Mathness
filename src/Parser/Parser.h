#pragma once

#include <fmt/core.h>
#include <fmt/color.h>
#include <string>
#include <tuple>
#include <vector>
#include <functional>
#include "./AST/Node.h"
#include "./AST/Operation Nodes/Binary/BinaryOperations.h"
#include "./AST/Operation Nodes/Unary/UnaryOperations.h"
#include "./AST/Value Nodes/ValueNodes.h"

#include "../Utils/TokenDefinitions.h"
#include "../Utils/TypeDefinitions.h"

struct NodeResult : public std::variant<std::shared_ptr<INode>, Error>
{
	using std::variant<std::shared_ptr<INode>, Error>::variant;

	constexpr bool IsNode() const
	{
		return std::holds_alternative<std::shared_ptr<INode>>(*this);
	}

	constexpr bool IsError() const
	{
		return std::holds_alternative<Error>(*this);
	}

	std::shared_ptr<INode> ToNode() const
	{
		assert(IsNode());
		return std::get<std::shared_ptr<INode>>(*this);
	}

	constexpr Error ToError() const
	{
		assert(IsError());
		return std::get<Error>(*this);
	}
};

enum class ExpressionElementType
{
	ODD,
	EVEN
};

class Parser
{
public:
	void Parse(std::vector<LexingToken>& lexingTokens);
	static Parser& GetInstance();

private:
	std::vector<LexingToken> _RValueTokenList;
	std::vector<LexingToken> _LValueTokenList;

	std::shared_ptr<std::vector<LexingToken>> _CurrentValueTokenList;

	std::vector<LexingToken>::iterator _CurrentTokenIt;

	//wanna create a list of functions that check the token type
	std::vector<std::function<bool(TokenType)>> _TokenTypeCheckList;

	std::optional<Error> CheckTokenType(TokenType tokenType) const;

	
	Parser() = default;

	void Init();

	std::optional<Error> SplitTokenList(std::vector<LexingToken>& lexingTokens);
	std::optional<Error> CheckBrackets(std::vector<LexingToken>& lexingTokens) const;


	
	NodeResult EvaluateExpression();
	NodeResult EvaluateMatrix();
	NodeResult EvaluateList();


	size_t CurrentTokenIndex() const;

};