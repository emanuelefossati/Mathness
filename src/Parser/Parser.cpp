#include "Parser.h"
#include <stack>

Parser& Parser::GetInstance()
{
	static Parser instance;
	return instance;
}

void Parser::Parse(std::vector<LexingToken>& lexingTokens)
{
	Init();

	auto error = SplitTokenList(lexingTokens);

	if (error.has_value())
	{
		fmt::print(fmt::fg(fmt::color::red), "{}\n", error.value());
		return;
	}
}

void Parser::Init()
{
	_RValueTokenList = std::vector<LexingToken>();
	_LValueTokenList = std::vector<LexingToken>();
}

std::optional<Error> Parser::CheckBrackets(std::vector<LexingToken>& lexingTokens) const
{
	enum BracketType {
		ROUND,
		SQUARE,
		CURLY
	};

	struct BracketData
	{
		BracketType Type;
		size_t Index;
	};

	std::stack<BracketData> openBrackets;

	for (size_t tokenIndex = 0; tokenIndex < lexingTokens.size(); tokenIndex++)
	{
		auto& token = lexingTokens[tokenIndex];
		
		if (token.Type == TokenType::OPEN_ROUND_BRACKET)
			openBrackets.emplace(BracketType::ROUND);

		else if (token.Type == TokenType::OPEN_SQUARE_BRACKET)
			openBrackets.emplace(BracketType::SQUARE);

		else if (token.Type == TokenType::OPEN_CURLY_BRACKET)
			openBrackets.emplace(BracketType::CURLY);

		//closing brackets

		else if (token.Type == TokenType::CLOSE_ROUND_BRACKET)
		{
			auto& lastOpenBracket = openBrackets.top();

			if (lastOpenBracket.Type != ROUND)
			{
				return Error("Invalid closing round bracket in this position", tokenIndex);
			}

			openBrackets.pop();
		}

		else if (token.Type == TokenType::CLOSE_SQUARE_BRACKET)
		{
			auto& lastOpenBracket = openBrackets.top();

			if (lastOpenBracket.Type != SQUARE)
			{
				return Error("Invalid closing square bracket in this position", tokenIndex);
			}

			openBrackets.pop();
		}

		else if (token.Type == TokenType::CLOSE_CURLY_BRACKET)
		{
			auto& lastOpenBracket = openBrackets.top();

			if (lastOpenBracket.Type != CURLY)
			{
				return Error("Invalid closing curly bracket in this position", tokenIndex);
			}

			openBrackets.pop();
		}
	}

	if (!openBrackets.empty())
	{
		return Error("Unclosed bracket", Range(openBrackets.top().Index));
	}

	return std::nullopt;
}

std::optional<Error> Parser::SplitTokenList(std::vector<LexingToken>& lexingTokens)
{
	size_t equalsCount = 0;

	for (size_t tokenIndex = 0; tokenIndex < lexingTokens.size(); tokenIndex++)
	{
		auto& token = lexingTokens[tokenIndex];
		if (token.Type == TokenType::EQUALS)
		{
			equalsCount++;
			continue;
		}

		if (equalsCount == 0)
			_LValueTokenList.push_back(token);
		else if (equalsCount == 1)
			_LValueTokenList.push_back(token);
		else
			return Error("Multiple equals signs found", Range(tokenIndex-1, 1));
	}

	if (equalsCount == 0)
		return Error("No equals sign found");

	return std::nullopt;
}

size_t Parser::CurrentTokenIndex() const
{
	assert(_CurrentValueTokenList != nullptr);
	assert(_CurrentTokenIt != _CurrentValueTokenList->end());

	return std::distance(_CurrentValueTokenList->begin(), _CurrentTokenIt);
}

NodeResult Parser::ParseExpression()
{
	std::shared_ptr<INode> rootNode = nullptr;
	std::shared_ptr<INode> currentNode = nullptr;

	auto evenList =
	{
		&Parser::CheckForOpenRoundBracket,
		&Parser::CheckForOpenSquareBracket,
		&Parser::CheckForValue,
		&Parser::CheckForUnaryFunctionName,
		&Parser::CheckForBinaryFunctionName,
		&Parser::CheckForMinus
	};

	auto checkFunctionIt = evenList.begin();
	for (; checkFunctionIt != evenList.end(); checkFunctionIt++)
	{
		auto result = (this->*(*checkFunctionIt))(rootNode);

		if (result.IsError())
			return result.ToError();

		else if (result.ToBool())
			break;
	}

	if (checkFunctionIt == evenList.end())
		return Error("Invalid token", Range(CurrentTokenIndex(), 1));

	int previousPriority = 4;

	currentNode = rootNode;

	while (!IsTokenExpressionEnd(_CurrentTokenIt->Type))
	{
		std::shared_ptr<IBinaryNode> operationNode = nullptr;

		auto [result, priority] = CheckForArithmeticOperator(operationNode);

		if (result.IsError())
			return result.ToError();

		std::shared_ptr<INode> rightNode = nullptr;

		auto checkFunctionIt = evenList.begin();
		for (; checkFunctionIt != evenList.end(); checkFunctionIt++)
		{
			auto result = (this->*(*checkFunctionIt))(rightNode);

			if (result.IsError())
				return result.ToError();

			else if (result.ToBool())
				break;
		}

		if (checkFunctionIt == evenList.end())
			return Error("Invalid token", Range(CurrentTokenIndex(), 1));


		if (priority <= previousPriority)
		{

		}
		else
		{

		}

		previousPriority = priority;
	}

	return rootNode;
}

NodeResult Parser::ParseMatrix()
{
	assert(_CurrentTokenIt->Type == TokenType::OPEN_SQUARE_BRACKET);

	std::shared_ptr<MatrixNode> matrixNode = std::make_shared<MatrixNode>();
	std::vector<std::shared_ptr<INode>> elements;

	bool firstRowDone = false;
	
	size_t matrixColumnCount = 0;
	size_t matrixRowCount = 0;
	
	size_t currentColumnCount = 0;


	while (true)
	{
		NodeResult currentElementExpressionResult = ParseExpression();

		if (currentElementExpressionResult.IsError())
			return currentElementExpressionResult;

		std::shared_ptr<INode> currentElement = currentElementExpressionResult.ToNode();

		elements.push_back(currentElement);

		if(_CurrentTokenIt->Type != TokenType::COMMA && 
			_CurrentTokenIt->Type != TokenType::SEMICOLON && 
			_CurrentTokenIt->Type != TokenType::CLOSE_SQUARE_BRACKET)
			return Error("Invalid token", Range(CurrentTokenIndex(), 1));

		elements.push_back(currentElement);

		if (!firstRowDone)
		{
			matrixColumnCount++;

			if (_CurrentTokenIt->Type == TokenType::SEMICOLON)
			{
				firstRowDone = true;
				matrixRowCount++;

				assert(currentColumnCount == 0);
			}
			else if (_CurrentTokenIt->Type == TokenType::CLOSE_SQUARE_BRACKET)
			{
				matrixRowCount++;

				assert(matrixColumnCount > 0);
				assert(matrixRowCount == 1);

				matrixNode->SetElements(elements);
				matrixNode->SetRows(matrixRowCount);
				matrixNode->SetColumns(matrixColumnCount);

				_CurrentTokenIt++;

				return matrixNode;
			}
		}
		else
		{
			currentColumnCount++;

			if (currentColumnCount > matrixColumnCount)
			{
				return Error("Invalid matrix size delimiter", Range(CurrentTokenIndex(), 1));
			}

			if (_CurrentTokenIt->Type == TokenType::SEMICOLON)
			{
				if (currentColumnCount != matrixColumnCount)
				{
					return Error("Invalid matrix size delimiter", Range(CurrentTokenIndex(), 1));
				}

				matrixRowCount++;
				currentColumnCount = 0;
			}
			else if (_CurrentTokenIt->Type == TokenType::CLOSE_SQUARE_BRACKET)
			{
				assert(matrixColumnCount > 0);
				assert(matrixRowCount > 1);

				if (currentColumnCount != matrixColumnCount)
				{
					return Error("Invalid matrix size delimiter", Range(CurrentTokenIndex(), 1));
				}

				matrixRowCount++;

				matrixNode->SetElements(elements);
				matrixNode->SetRows(matrixRowCount);
				matrixNode->SetColumns(matrixColumnCount);

				_CurrentTokenIt++;

				return matrixNode;
			}
		}
	}
}

NodeResult Parser::ParseList()
{
	return NodeResult();
}


std::optional<Error> Parser::CheckTokenType(TokenType tokenType) const
{
	bool isTokenTypeValid = false;
	for (auto& checkFunction : _ExpectedTokenTypes)
	{
		if (checkFunction(_CurrentTokenIt->Type))
		{
			isTokenTypeValid = true;
			break;
		}
	}

	if (!isTokenTypeValid)
		return Error("Invalid token type in this position", Range(CurrentTokenIndex(), 1));

	return std::nullopt;
}


ParsingCheckResult Parser::CheckForOpenRoundBracket(std::shared_ptr<INode>& node)
{
	if (_CurrentTokenIt->Type == TokenType::OPEN_ROUND_BRACKET)
	{
		_CurrentTokenIt++;
		auto expressionResult = ParseExpression();

		if (expressionResult.IsError())
			return expressionResult.ToError();

		if((_CurrentTokenIt - 1)->Type != TokenType::CLOSE_ROUND_BRACKET)
			return Error("Expected closing round bracket after inner expression", Range(CurrentTokenIndex() - 1, 1));

		node = expressionResult.ToNode();

		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForOpenSquareBracket(std::shared_ptr<INode>& node)
{
	if (_CurrentTokenIt->Type == TokenType::OPEN_SQUARE_BRACKET)
	{
		_CurrentTokenIt++;
		auto expressionResult = ParseMatrix();

		if (expressionResult.IsError())
			return expressionResult.ToError();

		if((_CurrentTokenIt - 1)->Type != TokenType::CLOSE_SQUARE_BRACKET)
			return Error("Expected closing square bracket after matrix", Range(CurrentTokenIndex() - 1, 1));

		node = expressionResult.ToNode();

		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForOpenCurlyBracket(std::shared_ptr<INode>& node)
{
	if (_CurrentTokenIt->Type == TokenType::OPEN_CURLY_BRACKET)
	{
		_CurrentTokenIt++;
		auto expressionResult = ParseList();

		if (expressionResult.IsError())
			return expressionResult.ToError();

		if((_CurrentTokenIt - 1)->Type != TokenType::CLOSE_CURLY_BRACKET)
			return Error("Expected closing curly bracket after list", Range(CurrentTokenIndex() - 1, 1));

		node = expressionResult.ToNode();

		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForValue(std::shared_ptr<INode>& node)
{
	if (IsTokenValue(_CurrentTokenIt->Type))
	{
		node = std::make_shared<ScalarNode>(GetScalarValue(*_CurrentTokenIt));
		_CurrentTokenIt++;
		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForUnaryFunctionName(std::shared_ptr<INode>& node)
{
	if (IsTokenUnaryFunctionName(_CurrentTokenIt->Type))
	{
		auto unaryFunction = CreateUnaryFunction(_CurrentTokenIt->Type);
		_CurrentTokenIt++;

		auto expressionResult = ParseExpression();

		if (expressionResult.IsError())
			return expressionResult.ToError();

		if((_CurrentTokenIt - 1)->Type != TokenType::CLOSE_ROUND_BRACKET)
			return Error("Expected closing round bracket after unary function", Range(CurrentTokenIndex() - 1, 1));

		unaryFunction->SetChild(expressionResult.ToNode());
		node = unaryFunction;

		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForBinaryFunctionName(std::shared_ptr<INode>& node)
{
	if (IsTokenBinaryFunctionName(_CurrentTokenIt->Type))
	{
		auto binaryFunction = CreateBinaryNode(_CurrentTokenIt->Type);
		_CurrentTokenIt++;

		auto leftExpressionResult = ParseExpression();

		if (leftExpressionResult.IsError())
			return leftExpressionResult.ToError();

		if ((_CurrentTokenIt - 1)->Type != TokenType::COMMA)
			return Error("Expected comma between child expressions inside binary function", Range(CurrentTokenIndex() -1, 1));

		_CurrentTokenIt++;

		auto rightExpressionResult = ParseExpression();

		if (rightExpressionResult.IsError())
			return rightExpressionResult.ToError();

		if((_CurrentTokenIt - 1)->Type != TokenType::CLOSE_ROUND_BRACKET)
			return Error("Expected closing round bracket after binary function", Range(CurrentTokenIndex() -1, 1));

		binaryFunction->SetLeft(leftExpressionResult.ToNode());
		binaryFunction->SetRight(rightExpressionResult.ToNode());

		node = binaryFunction;

		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForMinus(std::shared_ptr<INode>& node)
{
	if (IsTokenMinus(_CurrentTokenIt->Type))
	{
		auto list =
		{
			&Parser::CheckForOpenRoundBracket,
			&Parser::CheckForOpenSquareBracket,
			&Parser::CheckForValue,
			&Parser::CheckForUnaryFunctionName,
			&Parser::CheckForBinaryFunctionName
		};

		std::shared_ptr<INode> negatedNode = nullptr;

		auto checkFunctionIt = list.begin();
		for (; checkFunctionIt != list.end(); checkFunctionIt++)
		{
			auto result = (this->*(*checkFunctionIt))(negatedNode);

			if (result.IsError())
				return result;

			else if (result.ToBool())
				break;
		}

		if (checkFunctionIt == list.end())
			return Error("Invalid token after minus sign", Range(CurrentTokenIndex(), 1));

		assert(negatedNode != nullptr);

		auto minusNode = std::make_shared<ProductNode>();
		minusNode->SetLeft(std::make_shared<ScalarNode>(-1));
		minusNode->SetRight(negatedNode);

		node = minusNode;

		return true;
	}

	return false;
}

std::tuple<ParsingCheckResult, int> Parser::CheckForArithmeticOperator(std::shared_ptr<IBinaryNode>& node)
{
	if (IsTokenArithmeticOperator(_CurrentTokenIt->Type))
	{
		auto binaryOperation = CreateBinaryNode(_CurrentTokenIt->Type);
		_CurrentTokenIt++;

		node = binaryOperation;

		int priority = GetOperationPriority(_CurrentTokenIt->Type);

		return std::make_tuple(true, priority);
	}

	return std::make_tuple(Error("Expected an operation symbol"), -1);
}