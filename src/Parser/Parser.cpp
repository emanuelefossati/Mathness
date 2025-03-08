#include "Parser.h"
#include <stack>

Parser& Parser::GetInstance()
{
	static Parser instance;
	return instance;
}

NodeResult Parser::Parse(std::vector<LexingToken>& lexingTokens)
{
	auto error = CheckBrackets(lexingTokens);

	if (error.has_value())
		return error.value();

	auto& lastToken = lexingTokens.back();

	if (IsTokenArithmeticOperator(lastToken.Type) || 
		IsTokenOpenBracket(lastToken.Type) || 
		IsTokenUnaryFunctionName(lastToken.Type) || 
		IsTokenBinaryFunctionName(lastToken.Type))
	{
		return Error("Invalid token at the end of the expression");
	}
	
	_CurrentTokenIt = lexingTokens.begin();
	_TokenItEnd = lexingTokens.end();

	return ParseExpression();	
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

std::tuple<std::vector<LexingToken>, std::vector<LexingToken>, std::optional<Error>> Parser::SplitTokenList(std::vector<LexingToken>& lexingTokens)
{
	std::vector<LexingToken>::iterator tokenIt = lexingTokens.begin();
	
	std::vector<LexingToken> _LValueTokenList;
	std::vector<LexingToken> _RValueTokenList;

	for (; tokenIt != lexingTokens.end() && tokenIt->Type != TokenType::EQUALS; tokenIt++)
		_LValueTokenList.push_back(*tokenIt);

	if (tokenIt == lexingTokens.end())
		return std::make_tuple(_LValueTokenList, _RValueTokenList, std::nullopt);

	//skip the equals sign
	tokenIt++;

	for (; tokenIt != lexingTokens.end() && tokenIt->Type != TokenType::EQUALS; tokenIt++)
		_RValueTokenList.push_back(*tokenIt);

	if (tokenIt == lexingTokens.end())
		return std::make_tuple(_LValueTokenList, _RValueTokenList, std::nullopt);

	return std::make_tuple(_LValueTokenList, _RValueTokenList, Error("Multiple equals signs found"));
}

size_t Parser::CurrentTokenIndex() const
{
	assert(_CurrentTokenIt != _TokenItEnd);

	//return std::distance(_CurrentValueTokenList->begin(), _CurrentTokenIt);

	return 0;
}

NodeResult Parser::ParseExpression()
{
	std::shared_ptr<INode> rootNode = nullptr;
	std::shared_ptr<INode> currentNode = nullptr;

	auto evenList =
	{
		&Parser::CheckForUnaryFunctionName,
		&Parser::CheckForBinaryFunctionName,

		&Parser::CheckForOpenRoundBracket,
		&Parser::CheckForOpenSquareBracket,
		&Parser::CheckForValue,

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

	while (_CurrentTokenIt != _TokenItEnd && !IsTokenExpressionEnd(_CurrentTokenIt->Type))
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
			//keep going up the tree until we find a node with a lower priority
			while (true)
			{
				if (currentNode->GetParent() == nullptr)
				{
					break;
				}

				int parentOperationPriority = currentNode->GetParent()->GetPriority();
				assert(parentOperationPriority > 0);

				if (parentOperationPriority < priority)
				{
					break;
				}

				currentNode = currentNode->GetParent();
			}

			operationNode->SetLeft(currentNode);
			operationNode->SetRight(rightNode);


			if (currentNode->GetParent() != nullptr)
			{
				assert(std::dynamic_pointer_cast<IBinaryNode>(currentNode->GetParent()) != nullptr);
				std::shared_ptr<IBinaryNode> parent = std::static_pointer_cast<IBinaryNode>(currentNode->GetParent());

				parent->SetRight(operationNode);
				operationNode->SetParent(parent);
			}

			currentNode->SetParent(operationNode);
			rightNode->SetParent(operationNode);
		}
		else
		{
			assert(std::dynamic_pointer_cast<IBinaryNode>(currentNode) != nullptr);
			std::shared_ptr<IBinaryNode> currentBinaryNode = std::static_pointer_cast<IBinaryNode>(currentNode);

			auto previousRight = currentBinaryNode->GetRight();

			operationNode->SetLeft(previousRight);
			operationNode->SetRight(rightNode);

			previousRight->SetParent(operationNode);
			rightNode->SetParent(operationNode);

			operationNode->SetParent(currentNode);
			currentBinaryNode->SetRight(operationNode);
		}

		currentNode = operationNode;

		if (currentNode->GetParent() == nullptr)
			rootNode = currentNode;
		
		previousPriority = priority;
	}

	return rootNode;
}

NodeResult Parser::ParseMatrix()
{
	assert((_CurrentTokenIt)->Type == TokenType::OPEN_SQUARE_BRACKET);
	
	_CurrentTokenIt++;

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
			return Error("Invalid token after matrix element", Range(CurrentTokenIndex(), 1));

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

				if (currentColumnCount != matrixColumnCount)
				{
					return Error("Invalid matrix size delimiter", Range(CurrentTokenIndex(), 1));
				}

				matrixRowCount++;

				matrixNode->SetElements(elements);
				matrixNode->SetRows(matrixRowCount);
				matrixNode->SetColumns(matrixColumnCount);

				

				return matrixNode;
			}
		}

		_CurrentTokenIt++;
	}
}

NodeResult Parser::ParseList()
{
	return NodeResult();
}

ParsingCheckResult Parser::CheckForOpenRoundBracket(std::shared_ptr<INode>& node)
{
	if (_CurrentTokenIt->Type == TokenType::OPEN_ROUND_BRACKET)
	{
		_CurrentTokenIt++;
		auto expressionResult = ParseExpression();

		if (expressionResult.IsError())
			return expressionResult.ToError();

		if(_CurrentTokenIt->Type != TokenType::CLOSE_ROUND_BRACKET)
			return Error("Expected closing round bracket after inner expression", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

		node = expressionResult.ToNode();

		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForOpenSquareBracket(std::shared_ptr<INode>& node)
{
	if (_CurrentTokenIt->Type == TokenType::OPEN_SQUARE_BRACKET)
	{
		auto expressionResult = ParseMatrix();

		if (expressionResult.IsError())
			return expressionResult.ToError();

		if(_CurrentTokenIt->Type != TokenType::CLOSE_SQUARE_BRACKET)
			return Error("Expected closing square bracket after matrix", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

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

		if(_CurrentTokenIt->Type != TokenType::CLOSE_CURLY_BRACKET)
			return Error("Expected closing curly bracket after list", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

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

ParsingCheckResult Parser::CheckForIdentifier(std::shared_ptr<INode>& node)
{
	return false;
}

ParsingCheckResult Parser::CheckForUnaryFunctionName(std::shared_ptr<INode>& node)
{
	if (IsTokenUnaryFunctionName(_CurrentTokenIt->Type))
	{
		auto unaryFunction = CreateUnaryFunction(_CurrentTokenIt->Type);
		_CurrentTokenIt++;

		if (_CurrentTokenIt->Type != TokenType::OPEN_ROUND_BRACKET)
			return Error("Expected opening round bracket after function declaration", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

		auto expressionResult = ParseExpression();

		if (expressionResult.IsError())
			return expressionResult.ToError();

		if((_CurrentTokenIt)->Type != TokenType::CLOSE_ROUND_BRACKET)
			return Error("Expected closing round bracket after unary function", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

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

		if (_CurrentTokenIt->Type != TokenType::OPEN_ROUND_BRACKET)
			return Error("Expected opening round bracket after function declaration", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

		auto leftExpressionResult = ParseExpression();

		if (leftExpressionResult.IsError())
			return leftExpressionResult.ToError();

		if (_CurrentTokenIt->Type != TokenType::COMMA)
			return Error("Expected comma between child expressions inside binary function", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

		auto rightExpressionResult = ParseExpression();

		if (rightExpressionResult.IsError())
			return rightExpressionResult.ToError();

		if(_CurrentTokenIt->Type != TokenType::CLOSE_ROUND_BRACKET)
			return Error("Expected closing round bracket after binary function", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

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
		_CurrentTokenIt++;

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

		node = binaryOperation;

		int priority = GetOperationTokenPriority(_CurrentTokenIt->Type);
		_CurrentTokenIt++;

		return std::make_tuple(true, priority);
	}

	return std::make_tuple(Error("Expected an operation symbol"), -1);
}