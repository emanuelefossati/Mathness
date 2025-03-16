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
	static Parser& GetInstance();

	NodeResult Parse(std::vector<LexingToken>& lexingTokens);
	NodeResult ParseLValue(std::vector<LexingToken>& lexingTokens);

	std::tuple<std::vector<LexingToken>, std::vector<LexingToken>, std::optional<Error>> SplitTokenList(std::vector<LexingToken>& lexingTokens);




private:
	std::shared_ptr<std::vector<LexingToken>> _CurrentValueTokenList;

	std::vector<LexingToken>::iterator _TokenItBegin;
	std::vector<LexingToken>::iterator _TokenItEnd;
	std::vector<LexingToken>::iterator _CurrentTokenIt;
	
	Parser() = default;

	void Init();

	std::optional<Error> CheckBrackets() const;
	
	NodeResult ParseExpression();
	NodeResult ParseMatrix();
	NodeResult ParseList();


	ParsingCheckResult CheckForOpenRoundBracket(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForOpenSquareBracket(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForOpenCurlyBracket(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForValue(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForIdentifier(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForMinus(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForUnaryFunctionName(std::shared_ptr<INode>& node);
	ParsingCheckResult CheckForBinaryFunctionName(std::shared_ptr<INode>& node);

	std::tuple<ParsingCheckResult, int> CheckForArithmeticOperator(std::shared_ptr<IBinaryNode>& node);

	size_t CurrentTokenIndex() const;

	std::tuple<std::optional<std::string>, std::optional<std::string>> GetTokenSymbolsBetween(std::vector<LexingToken>::iterator actualTokenIt) const;
	Error CreateErrorMessageWithAdjacentTokens(std::string message, std::vector<LexingToken>::iterator actualTokenIt) const;




};