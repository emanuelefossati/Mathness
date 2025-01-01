#pragma once

#include <string>
#include <span>
#include "TokenDefinitions.h"



class Lexer
{
public:
	std::tuple<std::vector<LexingToken>, std::string>Lex(std::string& input);
	static Lexer& GetInstance();

private:
	Lexer()  = default;

	std::string _Input;
	int _Index = 0;
	std::vector<LexingToken> _Tokens;
	
	void Init(std::string& input);
	void ClearComments();

	void CheckForNumber();
	void CheckForIdentifier();
	bool CheckForSymbol();
};

