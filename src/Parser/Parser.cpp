#include "Parser.h"

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
	int currentRoundDepth = 0;
	int currentSquareDepth = 0;
	int currentCurlyDepth = 0;

	for (size_t tokenIndex = 0; tokenIndex < lexingTokens.size(); tokenIndex++)
	{
		auto& token = lexingTokens[tokenIndex];

		if (token.Type == TokenType::OPEN_ROUND_BRACKET)
			currentRoundDepth++;

		else if (token.Type == TokenType::CLOSE_ROUND_BRACKET)
			currentRoundDepth--;

		else if (token.Type == TokenType::OPEN_SQUARE_BRACKET)
			currentSquareDepth++;
		
		else if (token.Type == TokenType::CLOSE_SQUARE_BRACKET)
			currentSquareDepth--;
		
		else if (token.Type == TokenType::OPEN_CURLY_BRACKET)
			currentCurlyDepth++;

		else if (token.Type == TokenType::CLOSE_CURLY_BRACKET)
			currentCurlyDepth--;


		if (currentRoundDepth < 0)
			return Error("Invalid round bracket setting", Range(tokenIndex, 1));
		
		else if (currentSquareDepth < 0)
			return Error("Invalid square bracket setting", Range(tokenIndex, 1));

		else if (currentCurlyDepth < 0)
			return Error("Invalid curly bracket setting", Range(tokenIndex, 1));

		// Check for empty brackets
		if (tokenIndex < lexingTokens.size() - 1)
		{
			auto& nextToken = lexingTokens[tokenIndex + 1];

			if (token.Type == TokenType::OPEN_ROUND_BRACKET && nextToken.Type == TokenType::CLOSE_ROUND_BRACKET)
				return Error("Empty round brackets", Range(tokenIndex, 2));

			else if (token.Type == TokenType::OPEN_SQUARE_BRACKET && nextToken.Type == TokenType::CLOSE_SQUARE_BRACKET)
				return Error("Empty square brackets", Range(tokenIndex, 2));

			else if (token.Type == TokenType::OPEN_CURLY_BRACKET && nextToken.Type == TokenType::CLOSE_CURLY_BRACKET)
				return Error("Empty curly brackets", Range(tokenIndex, 2));
		}

	}

	if (currentRoundDepth != 0)
		return Error("Invalid round bracket setting");

	else if (currentSquareDepth != 0)
		return Error("Invalid square bracket setting");

	else if (currentCurlyDepth != 0)
		return Error("Invalid curly bracket setting");

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

NodeResult Parser::BuildTree(size_t currentTokenIndex)
{
	std::shared_ptr<INode> root = nullptr;
	std::shared_ptr<INode> current = root;

	while (true)
	{
		auto& currentToken = _RValueTokenList[currentTokenIndex];
		
		//first token in an expression can't be an operator (except for the unary minus or plus) nor comma, semicolon, or colon

		if (currentToken.Type != TokenType::MINUS && IsTokenArithmeticOperator(currentToken.Type) ||
			currentToken.Type == TokenType::COMMA ||
			currentToken.Type == TokenType::COLON ||
			currentToken.Type == TokenType::SEMICOLON)
		{
			return Error("Invalid expression start", Range(currentTokenIndex, 1));
		}

		if (currentToken.Type == TokenType::MINUS)
		{
			//create a product node with a value node of -1 and the next node

			continue;
		}

		if (currentToken.Type == TokenType::NUMBER)
		{

		}
		else if (currentToken.Type == TokenType::PI || currentToken.Type == TokenType::E)
		{

		}

		else if(IsTokenElementaryFunction(currentToken.Type))
		{

		}
		else if (currentToken.Type == TokenType::STORAGE)
		{

		}
		else if (currentToken.Type == TokenType::OPEN_ROUND_BRACKET)
		{
		}
		else if (currentToken.Type == TokenType::OPEN_SQUARE_BRACKET)
		{
		}
		else if (currentToken.Type == TokenType::OPEN_CURLY_BRACKET)
		{
		}
		else
		{
			return Error("Invalid token", Range(currentTokenIndex, 1));
		}

		currentTokenIndex++;

		//now we have to check if the next token is an operator or the end of the expression





	}

	return root;
}

