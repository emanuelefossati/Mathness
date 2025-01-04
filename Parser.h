#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "TokenDefinitions.h"
#include "Utils.h"

struct ParsingToken : LexingToken 
{
	int Depth = 0;

	ParsingToken(LexingToken token, int depth) : LexingToken(token), Depth(depth) {}
};

class Parser
{
public:
	void Parse(std::vector<LexingToken>& lexingTokens);
	static Parser& GetInstance();

private:
	std::vector<ParsingToken> _Tokens;
	std::optional<std::string> _LValue;
	
	Parser() = default;

	void Init();

	std::optional<error_t> CheckAssignment();
	std::optional<error_t> CheckBrackets(std::vector<LexingToken>& lexingTokens);
	
	void BuildTree();

};