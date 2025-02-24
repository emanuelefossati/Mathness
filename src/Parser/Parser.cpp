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
	std::shared_ptr<INode> fatherNode = nullptr;
	std::shared_ptr<INode> leftChildNode = nullptr;
	std::shared_ptr<INode> rightChildNode = nullptr;

	if (_CurrentTokenIt->Type == TokenType::OPEN_ROUND_BRACKET)
	{
		_CurrentTokenIt++;
		auto expressionResult = ParseExpression();

		if (expressionResult.IsError())
			return expressionResult;

		assert(_CurrentTokenIt->Type == TokenType::CLOSE_ROUND_BRACKET);
		
		leftChildNode = expressionResult.ToNode();

	}
	else if (_CurrentTokenIt->Type == TokenType::OPEN_SQUARE_BRACKET)
	{
		_CurrentTokenIt++;
		auto expressionResult = ParseMatrix();

		if (expressionResult.IsError())
			return expressionResult;

		assert(_CurrentTokenIt->Type == TokenType::CLOSE_SQUARE_BRACKET);

		leftChildNode = expressionResult.ToNode();
	}
	else if (_CurrentTokenIt->Type == TokenType::OPEN_CURLY_BRACKET)
	{
		_CurrentTokenIt++;
		auto expressionResult = ParseList();

		if (expressionResult.IsError())
			return expressionResult;

		assert(_CurrentTokenIt->Type == TokenType::CLOSE_CURLY_BRACKET);

		leftChildNode = expressionResult.ToNode();
	}
	else if (IsTokenValue(_CurrentTokenIt->Type))
	{
		leftChildNode = std::make_shared<ScalarNode>(GetScalarValue(*_CurrentTokenIt));
		_CurrentTokenIt++;
	}
	else if (IsTokenMinus(_CurrentTokenIt->Type))
	{
		_CurrentTokenIt++;

		if (_CurrentTokenIt->Type == TokenType::OPEN_ROUND_BRACKET)
		{

		}
		else if (_CurrentTokenIt->Type == TokenType::OPEN_SQUARE_BRACKET)
		{

		}
		else if (IsTokenValue(_CurrentTokenIt->Type))
		{

		}
		else if (IsTokenUnaryFunctionName(_CurrentTokenIt->Type))
		{
			//parse unary function
		}
		else if (IsTokenBinaryFunctionName(_CurrentTokenIt->Type))
		{
			//parse binary function
		}
		else if (_CurrentTokenIt->Type == TokenType::STORAGE)
		{
			//try to access the storage
		}
		else
		{
			return Error("Invalid token", Range(CurrentTokenIndex(), 1));
		}
	}
	else if (IsTokenUnaryFunctionName(_CurrentTokenIt->Type))
	{
		//parse unary function
	}
	else if (IsTokenBinaryFunctionName(_CurrentTokenIt->Type))
	{
		//parse binary function
	}
	else if (_CurrentTokenIt->Type == TokenType::STORAGE)
	{
		//try to access the storage
	}
	else
	{
		return Error("Invalid token", Range(CurrentTokenIndex(), 1));
	}




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

		assert(_CurrentTokenIt->Type == TokenType::CLOSE_ROUND_BRACKET);

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

		assert(_CurrentTokenIt->Type == TokenType::CLOSE_SQUARE_BRACKET);

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

		assert(_CurrentTokenIt->Type == TokenType::CLOSE_CURLY_BRACKET);

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
		//parse unary function

		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForBinaryFunctionName(std::shared_ptr<INode>& node)
{
	if (IsTokenBinaryFunctionName(_CurrentTokenIt->Type))
	{
		//parse binary function

		return true;
	}

	return false;
}

ParsingCheckResult Parser::CheckForMinus(std::shared_ptr<INode>& node)
{
	if (IsTokenMinus(_CurrentTokenIt->Type))
	{
		

		return true;
	}

	return false;
}