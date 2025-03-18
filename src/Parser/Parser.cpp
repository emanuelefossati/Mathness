#include "Parser.h"
#include <stack>

Parser& Parser::GetInstance()
{
	static Parser instance;
	return instance;
}

NodeResult Parser::Parse(std::vector<LexingToken>& lexingTokens)
{
	_TokenItBegin = lexingTokens.begin();
	_CurrentTokenIt = lexingTokens.begin();
	_TokenItEnd = lexingTokens.end();

	auto error = CheckBrackets();

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
	


	return ParseExpression();	
}

NodeResult Parser::ParseLValue(std::vector<LexingToken>& lexingTokens)
{
	_TokenItBegin = lexingTokens.begin();
	_CurrentTokenIt = lexingTokens.begin();
	_TokenItEnd = lexingTokens.end();

	auto error = CheckBrackets();

	if (error.has_value())
		return error.value();

	std::shared_ptr<INode> identifierNode = nullptr;

	ParsingCheckResult result = CheckForIdentifier(identifierNode);

	if (result.IsError())
		return result.ToError();

	if (!result.ToBool())
		return Error("Expected an identifier at the beginning of left value expression");

	return identifierNode;

}

std::optional<Error> Parser::CheckBrackets() const
{
	enum BracketType {
		ROUND,
		SQUARE,
		CURLY
	};

	struct BracketData
	{
		BracketType Type;
		std::vector<LexingToken>::iterator  IndexIt;
	};

	std::stack<BracketData> openBrackets;

	for (std::vector<LexingToken>::iterator index = _TokenItBegin; index != _TokenItEnd; index++)
	{
		auto& token = *index;
		
		if (token.Type == TokenType::OPEN_ROUND_BRACKET)
			openBrackets.emplace(BracketType::ROUND, index);

		else if (token.Type == TokenType::OPEN_SQUARE_BRACKET)
			openBrackets.emplace(BracketType::SQUARE, index);

		else if (token.Type == TokenType::OPEN_CURLY_BRACKET)
			openBrackets.emplace(BracketType::CURLY, index);

		//closing brackets

		else if (token.Type == TokenType::CLOSE_ROUND_BRACKET)
		{
			if (openBrackets.empty())
			{
				return CreateErrorMessageWithAdjacentTokens("Unmatched ')'", index);
			}

			auto& lastOpenBracket = openBrackets.top();

			if (lastOpenBracket.Type != ROUND)
			{
				return CreateErrorMessageWithAdjacentTokens("Invalid ')'", index);
			}

			openBrackets.pop();
		}

		else if (token.Type == TokenType::CLOSE_SQUARE_BRACKET)
		{
			if (openBrackets.empty())
			{
				return CreateErrorMessageWithAdjacentTokens("Unmatched ']'", index);
			}

			auto& lastOpenBracket = openBrackets.top();

			if (lastOpenBracket.Type != SQUARE)
			{
				return CreateErrorMessageWithAdjacentTokens("Invalid ']'", index);
			}

			openBrackets.pop();
		}

		else if (token.Type == TokenType::CLOSE_CURLY_BRACKET)
		{
			if (openBrackets.empty())
			{
				return CreateErrorMessageWithAdjacentTokens("Unmatched '}'", index);
			}

			auto& lastOpenBracket = openBrackets.top();

			if (lastOpenBracket.Type != CURLY)
			{
				return CreateErrorMessageWithAdjacentTokens("Invalid '}'", index);
			}



			openBrackets.pop();
		}
	}

	if (!openBrackets.empty())
	{
		return CreateErrorMessageWithAdjacentTokens("Unclosed bracket", openBrackets.top().IndexIt);
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
		&Parser::CheckForIdentifier,

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
		return CreateErrorMessageWithAdjacentTokens("Invalid token", _CurrentTokenIt);

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
			return CreateErrorMessageWithAdjacentTokens("Invalid token", _CurrentTokenIt);


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
			return CreateErrorMessageWithAdjacentTokens("Invalid token after matrix element", _CurrentTokenIt);

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
				return CreateErrorMessageWithAdjacentTokens("Invalid matrix size delimiter", _CurrentTokenIt);
			}

			if (_CurrentTokenIt->Type == TokenType::SEMICOLON)
			{
				if (currentColumnCount != matrixColumnCount)
				{
					return CreateErrorMessageWithAdjacentTokens("Invalid matrix size delimiter", _CurrentTokenIt);
				}

				matrixRowCount++;
				currentColumnCount = 0;
			}
			else if (_CurrentTokenIt->Type == TokenType::CLOSE_SQUARE_BRACKET)
			{
				assert(matrixColumnCount > 0);

				if (currentColumnCount != matrixColumnCount)
				{
					return CreateErrorMessageWithAdjacentTokens("Invalid matrix size delimiter", _CurrentTokenIt);
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
	assert((_CurrentTokenIt)->Type == TokenType::OPEN_CURLY_BRACKET);

	_CurrentTokenIt++;

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
			return CreateErrorMessageWithAdjacentTokens("Expected ')' to close inner expression", _CurrentTokenIt);

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
			return CreateErrorMessageWithAdjacentTokens("Expected ']' to close matrix", _CurrentTokenIt);

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
			return CreateErrorMessageWithAdjacentTokens("Expected '}' to close list", _CurrentTokenIt);

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
	if (_CurrentTokenIt->Type == TokenType::STORAGE)
	{
		auto identifierNode = std::make_shared<IdentifierNode>(_CurrentTokenIt->Value);

		_CurrentTokenIt++;

		if (_CurrentTokenIt == _TokenItEnd || _CurrentTokenIt->Type != TokenType::OPEN_SQUARE_BRACKET)
		{
			node = identifierNode;
			return true;
		}

		do 
		{
			_CurrentTokenIt++;

			auto indexExpressionResult = ParseExpression();

			if (_CurrentTokenIt->Type != TokenType::CLOSE_SQUARE_BRACKET)
				return CreateErrorMessageWithAdjacentTokens("Expected ']' to close index expression", _CurrentTokenIt);

			identifierNode->AddIndexExpression(indexExpressionResult.ToNode());

			_CurrentTokenIt++;


		} while (_CurrentTokenIt != _TokenItEnd && _CurrentTokenIt->Type == TokenType::OPEN_SQUARE_BRACKET);

		node = identifierNode;

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

		if (_CurrentTokenIt->Type != TokenType::OPEN_ROUND_BRACKET)
			return CreateErrorMessageWithAdjacentTokens("Expected '(' to define function argument", _CurrentTokenIt);

		_CurrentTokenIt++;

		auto expressionResult = ParseExpression();

		if (expressionResult.IsError())
			return expressionResult.ToError();

		if((_CurrentTokenIt)->Type != TokenType::CLOSE_ROUND_BRACKET)
			return CreateErrorMessageWithAdjacentTokens("Expected ')' to end up function declaration", _CurrentTokenIt);

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
			return CreateErrorMessageWithAdjacentTokens("Expected '(' to define function argument", _CurrentTokenIt);

		_CurrentTokenIt++;

		auto leftExpressionResult = ParseExpression();

		if (leftExpressionResult.IsError())
			return leftExpressionResult.ToError();

		if (_CurrentTokenIt->Type != TokenType::COMMA)
			return CreateErrorMessageWithAdjacentTokens("Expected ',' to separate argument expressions inside binary function", _CurrentTokenIt);

		_CurrentTokenIt++;

		auto rightExpressionResult = ParseExpression();

		if (rightExpressionResult.IsError())
			return rightExpressionResult.ToError();

		if(_CurrentTokenIt->Type != TokenType::CLOSE_ROUND_BRACKET)
			return CreateErrorMessageWithAdjacentTokens("Expected ')' to end up function declaration", _CurrentTokenIt);

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
			&Parser::CheckForIdentifier,
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
			return CreateErrorMessageWithAdjacentTokens("Invalid token", _CurrentTokenIt);

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

	return std::make_tuple(CreateErrorMessageWithAdjacentTokens("Expected an operation symbol", _CurrentTokenIt), -1);
}

std::tuple<std::optional<std::string>, std::optional<std::string>> Parser::GetTokenSymbolsBetween(std::vector<LexingToken>::iterator actualTokenIt) const
{
	std::optional<std::string> previousToken = std::nullopt;
	std::optional<std::string> nextToken = std::nullopt;

	if (actualTokenIt > _TokenItBegin)
		previousToken = (actualTokenIt - 1)->Value;

	if (actualTokenIt < _TokenItEnd - 1)
		nextToken = (actualTokenIt + 1)->Value;

	return std::make_tuple(previousToken, nextToken);
}

Error Parser::CreateErrorMessageWithAdjacentTokens(std::string message, std::vector<LexingToken>::iterator actualTokenIt) const
{
	auto [previousToken, nextToken] = GetTokenSymbolsBetween(actualTokenIt);

	if (previousToken.has_value() && nextToken.has_value())
	{
		message += fmt::format(" between '{}' and '{}'", previousToken.value(), nextToken.value());
		return Error(message);
	}

	if (previousToken.has_value())
	{
		message += fmt::format(" after '{}'", previousToken.value());
		return Error(message);
	}

	if (nextToken.has_value())
	{
		message += fmt::format(" before '{}'", nextToken.value());
		return Error(message);
	}

	return Error(message);
}