#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "../Utils/TokenDefinitions.h"
#include "../Utils/TypeDefinitions.h"

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

	std::optional<Error> CheckAssignment();
	std::optional<Error> CheckBrackets(std::vector<LexingToken>& lexingTokens);
	
	void BuildTree();

};