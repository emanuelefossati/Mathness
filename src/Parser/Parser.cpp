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
			return Error("Invalid round bracket setting");
		
		else if (currentSquareDepth < 0)
			return Error("Invalid square bracket setting");

		else if (currentCurlyDepth < 0)
			return Error("Invalid curly bracket setting");

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

std::shared_ptr<INode> Parser::BuildTree(size_t currentTokenIndex)
{
	return nullptr;
}

