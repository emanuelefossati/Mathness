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

	//wanna create a list of functions that check the token type
	std::vector<std::function<bool(TokenType)>> _ExpectedTokenTypes;

	std::optional<Error> CheckTokenType(TokenType tokenType) const;

	
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

	static int GetArithmeticOperatorNodePriority(SumNode& node) { return 1; }
	static int GetArithmeticOperatorNodePriority(SubtractionNode& node) { return 1; }
	static int GetArithmeticOperatorNodePriority(ProductNode& node) { return 2; }
	static int GetArithmeticOperatorNodePriority(DivisionNode& node) { return 2; }
	static int GetArithmeticOperatorNodePriority(ModulusNode& node) { return 2; }
	static int GetArithmeticOperatorNodePriority(PowerNode& node) { return 3; }

	template<typename InvalidType>
	static int GetArithmeticOperatorNodePriority(InvalidType& node ) { return -1; }

	size_t CurrentTokenIndex() const;

};