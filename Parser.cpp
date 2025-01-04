#include "Parser.h"

Parser& Parser::GetInstance()
{
	static Parser instance;
	return instance;
}

void Parser::Parse(std::vector<LexingToken>& lexingTokens)
{
	Init();

	auto error = CheckBrackets(lexingTokens);

	if (error.has_value())
	{
		std::cout << error.value() << std::endl;
		return;
	}

	for (auto& token : _Tokens)
	{
		std::cout << token.ToString() << std::endl;
	}
}

void Parser::Init()
{
	_Tokens.clear();
	_LValue = std::nullopt;
}

std::optional<error_t> Parser::CheckBrackets(std::vector<LexingToken>& lexingTokens)
{
	int currentRoundDepth = 0;
	int currentSquareDepth = 0;
	int currentCurlyDepth = 0;

	for (auto& token : lexingTokens)
	{
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
			return error_t("Invalid round bracket setting");
		
		else if (currentSquareDepth < 0)
			return error_t("Invalid square bracket setting");

		else if (currentCurlyDepth < 0)
			return error_t("Invalid curly bracket setting");

		
		if (token.Type != TokenType::OPEN_ROUND_BRACKET && token.Type != TokenType::CLOSE_ROUND_BRACKET)
			_Tokens.emplace_back(token, currentRoundDepth);

	}

	if (currentRoundDepth != 0)
		return error_t("Invalid round bracket setting");

	else if (currentSquareDepth != 0)
		return error_t("Invalid square bracket setting");

	else if (currentCurlyDepth != 0)
		return error_t("Invalid curly bracket setting");

	return std::nullopt;
}

std::optional<error_t> Parser::CheckAssignment()
{
	int assignmentIndex = -1;
	int assignmentCount = 0;

	for (int i = 0; i < _Tokens.size(); i++)
	{
		if (_Tokens[i].Type == TokenType::EQUALS)
		{
			assignmentIndex = i;
			assignmentCount++;
		}
	}

	if (assignmentCount > 1)
		return error_t("Multiple assignments found");

	if (assignmentCount == 0)
		return std::nullopt;

	// One assignment found

	if (assignmentIndex != 1)
		return error_t("Invalid assignment position");

	if (_Tokens[0].Type != TokenType::STORAGE)
		return error_t("Expected an identifier before assignment");

	_LValue = _Tokens[0].Value;

	//Remove the first two tokens
	_Tokens.erase(_Tokens.begin(), _Tokens.begin() + 2);

	return std::nullopt;
}

