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

NodeResult Parser::EvaluateExpression()
{
	std::shared_ptr<INode> rootNode = nullptr;
	std::shared_ptr<INode> currentNode = nullptr;

	//first token in an expression must be either a number, a variable, a function, a minus, or an open bracket

	_TokenTypeCheckList = { IsTokenValue, IsTokenOpenBracket, IsTokenMinus, IsTokenElementaryFunctionName };

	auto error = CheckTokenType(_CurrentTokenIt->Type);
	if (error.has_value())
		return error.value();

	if (IsTokenScalarValue(_CurrentTokenIt->Type))
	{
		rootNode = std::make_shared<ScalarNode>(GetScalarValue(*_CurrentTokenIt));

		_TokenTypeCheckList = { IsTokenArithmeticOperator, IsTokenExpressionEnd };
	}

	else if (IsTokenMinus(_CurrentTokenIt->Type))
	{
		auto node = std::make_shared<ProductNode>();
		node->SetLeft(std::make_shared<ScalarNode>(-1.0));
		rootNode = node;


		_TokenTypeCheckList = { IsTokenValue, IsTokenOpenBracket, IsTokenElementaryFunctionName };
	}

	else if (IsTokenOpenBracket(_CurrentTokenIt->Type))
	{
		NodeResult result;
		_CurrentTokenIt++;

		if (_CurrentTokenIt->Type == TokenType::OPEN_ROUND_BRACKET)
		{
			result = EvaluateExpression();
		}
		else if (_CurrentTokenIt->Type == TokenType::OPEN_SQUARE_BRACKET)
		{
			result = EvaluateMatrix();
		}
		else if (_CurrentTokenIt->Type == TokenType::OPEN_CURLY_BRACKET)
		{
			result = EvaluateList();
		}

		if (result.IsError())
			return result;

		rootNode = result.ToNode();
	}

	else if (IsTokenUnaryFunctionName(_CurrentTokenIt->Type))
	{
		std::shared_ptr<IUnaryNode> unaryFunctionNode;

		switch (_CurrentTokenIt->Type)
		{
		case TokenType::ABS:
			unaryFunctionNode = std::make_shared<AbsoluteNode>();
			break;

		case TokenType::SQRT:
			unaryFunctionNode = std::make_shared<SqrtNode>();
			break;

		case TokenType::SIN:
			unaryFunctionNode = std::make_shared<SinNode>();
			break;

		case TokenType::COS:
			unaryFunctionNode = std::make_shared<CosNode>();
			break;

		case TokenType::TAN:
			unaryFunctionNode = std::make_shared<TanNode>();
			break;

		case TokenType::ASIN:
			unaryFunctionNode = std::make_shared<ArcsinNode>();
			break;

		case TokenType::ACOS:
			unaryFunctionNode = std::make_shared<ArccosNode>();
			break;

		case TokenType::ATAN:
			unaryFunctionNode = std::make_shared<ArctanNode>();
			break;

		case TokenType::LN:
			unaryFunctionNode = std::make_shared<LnNode>();
			break;

		case TokenType::EXP:
			unaryFunctionNode = std::make_shared<ExpNode>();
			break;

		default:
			assert(false && "Invalid unary function type");
		}

		_CurrentTokenIt++;

		if (_CurrentTokenIt->Type != TokenType::OPEN_ROUND_BRACKET)
			return Error("Expected open round bracket after unary function", Range(CurrentTokenIndex(), 1));

		NodeResult result = EvaluateExpression();

		if (result.IsError())
			return result;

		if (_CurrentTokenIt->Type != TokenType::CLOSE_ROUND_BRACKET)
			return Error("Expected close round bracket after unary function argument", Range(CurrentTokenIndex(), 1));

		unaryFunctionNode->SetChild(result.ToNode());

		rootNode = unaryFunctionNode;

		_TokenTypeCheckList = { IsTokenArithmeticOperator, IsTokenExpressionEnd };
	}

	else if (IsTokenBinaryFunctionName(_CurrentTokenIt->Type))
	{
		std::shared_ptr<IBinaryNode> binaryFunctionNode;

		switch (_CurrentTokenIt->Type)
		{
		case TokenType::SCALAR_PRODUCT:
			binaryFunctionNode = std::make_shared<DotNode>();
			break;

		case TokenType::VECTOR_PRODUCT:
			binaryFunctionNode = std::make_shared<CrossNode>();
			break;

		case TokenType::LOG:
			binaryFunctionNode = std::make_shared<LogNode>();
			break;

		default:
			assert(false && "Invalid binary function type");
		}

		_CurrentTokenIt++;

		if (_CurrentTokenIt->Type != TokenType::OPEN_ROUND_BRACKET)
			return Error("Expected open round bracket after binary function", Range(CurrentTokenIndex(), 1));

		NodeResult leftResult = EvaluateExpression();

		if (leftResult.IsError())
			return leftResult;

		if (_CurrentTokenIt->Type != TokenType::COMMA)
			return Error("Expected comma  after binary function", Range(CurrentTokenIndex(), 1));

		_CurrentTokenIt++;

		NodeResult rightResult = EvaluateExpression();

		if (rightResult.IsError())
			return rightResult;

		if (_CurrentTokenIt->Type != TokenType::CLOSE_ROUND_BRACKET)
			return Error("Expected close round bracket after binary function arguments", Range(CurrentTokenIndex(), 1));

		binaryFunctionNode->SetLeft(leftResult.ToNode());
		binaryFunctionNode->SetRight(rightResult.ToNode());

		rootNode = binaryFunctionNode;

		_TokenTypeCheckList = { IsTokenArithmeticOperator, IsTokenExpressionEnd };
	}

	currentNode = rootNode;
	
	_CurrentTokenIt++;


	while (true)
	{
		auto error = CheckTokenType(_CurrentTokenIt->Type);

		if (error.has_value())
			return error.value();

		if (IsTokenExpressionEnd(_CurrentTokenIt->Type))
		{
			return rootNode;
		}

		else if (IsTokenMinus)
		{

		}

		else if (IsTokenArithmeticOperator)
		{
			std::shared_ptr<IBinaryNode> binaryNode;

			switch (_CurrentTokenIt->Type)
			{
			case TokenType::PLUS:
				binaryNode = std::make_shared<SumNode>();
				break;

			case TokenType::MINUS:
				binaryNode = std::make_shared<SubtractionNode>();
				break;

			case TokenType::PRODUCT:
				binaryNode = std::make_shared<ProductNode>();
				break;

			case TokenType::DIVIDE:
				binaryNode = std::make_shared<DivisionNode>();
				break;

			case TokenType::POWER:
				binaryNode = std::make_shared<PowerNode>();
				break;

			default:
				assert(false && "Invalid binary operator type");
			}

			_CurrentTokenIt++;

			NodeResult rightResult = EvaluateExpression();

			if (rightResult.IsError())
				return rightResult;

			binaryNode->SetLeft(currentNode);
			binaryNode->SetRight(rightResult.ToNode());

			currentNode = binaryNode;
		}





		_CurrentTokenIt++;
	}

	return rootNode;
}

NodeResult Parser::EvaluateMatrix()
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
		NodeResult currentElementExpressionResult = EvaluateExpression();

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

NodeResult Parser::EvaluateList()
{
	return NodeResult();
}


std::optional<Error> Parser::CheckTokenType(TokenType tokenType) const
{
	bool isTokenTypeValid = false;
	for (auto& checkFunction : _TokenTypeCheckList)
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