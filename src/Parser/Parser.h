#pragma once

#include <fmt/core.h>
#include <fmt/color.h>
#include <string>
#include <tuple>
#include <vector>
#include <functional>

#include "./AST/Operation Nodes/Binary/BinaryOperations.h"
#include "./AST/Operation Nodes/Unary/UnaryOperations.h"
#include "./AST/Value Nodes/ValueNodes.h"

#include "../Utils/TokenDefinitions.h"
#include "../Utils/TypeDefinitions.h"
#include "./Utils/Utils.h"



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
	std::vector<LexingToken>::iterator _CurrentTokenItEnd;
	
	Parser() = default;

	void Init();

	std::optional<Error> SplitTokenList(std::vector<LexingToken>& lexingTokens);
	std::optional<Error> CheckBrackets(std::vector<LexingToken>& lexingTokens) const;
	
	NodeResult ParseExpression();
	NodeResult ParseMatrix();
	NodeResult ParseList();
	NodeResult ParseMathOperation();

	ParsingCheckResult CheckForOpenRoundBracket(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForOpenSquareBracket(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForOpenCurlyBracket(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForValue(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForMinus(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForUnaryFunctionName(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForBinaryFunctionName(std::shared_ptr<INode>& node);

	std::tuple<ParsingCheckResult, int> CheckForArithmeticOperator(std::shared_ptr<IBinaryNode>& node);

	size_t CurrentTokenIndex() const;



};