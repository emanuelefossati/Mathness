#include "Lexer.h"
#include <iostream>
#include <assert.h>
#include <format>

Lexer& Lexer::GetInstance()
{
	static Lexer instance;
	return instance;
}

void Lexer::Init(std::string& input)
{
	this->_Input = input;
	ClearComments();

	_Index = 0;
	_Tokens.clear();
}

void Lexer::ClearComments()
{
	size_t commentIndex = _Input.find("#");
	if (commentIndex != std::string::npos)
	{
		_Input = _Input.substr(0, commentIndex);
	}
}

void Lexer::CheckForNumber()
{
	int beginIndex = _Index;
	bool isDotFound = false;
	

	while (isdigit(_Input[_Index]) || (!isDotFound && _Input[_Index] == '.'))
	{
		if (_Input[_Index] == '.')
		{
			isDotFound = true;
		}

		_Index++;
	}
	
	if (beginIndex == _Index)
	{
		return;
	}

	_Tokens.emplace_back(TokenType::NUMBER, _Input.substr(beginIndex, (size_t) _Index - beginIndex));
}

bool Lexer::CheckForSymbol()
{
	auto& symbolMap = GetSymbolMap();
	auto it = symbolMap.find(_Input[_Index]);

	if (it == symbolMap.end())
	{
		return false;
	}

	_Tokens.emplace_back(it->second, std::string(1, _Input[_Index]));
	_Index++;

	return true;
}

void Lexer::CheckForIdentifier()
{
	int beginIndex = _Index;

	assert(!isdigit(_Input[beginIndex]));

	while (isalnum(_Input[_Index]) || _Input[_Index] == '_')
	{
		_Index++;
	}

	if (beginIndex == _Index)
	{
		return;
	}

	_Tokens.emplace_back(TokenType::IDENTIFIER, _Input.substr(beginIndex, (size_t)_Index - beginIndex));
}

std::tuple<std::vector<LexingToken>, std::string> Lexer::Lex(std::string& input)
{
	assert(input.size() > 0);

	Init(input);

	while (_Index < input.size())
	{
		if (isspace(input[_Index]))
		{
			_Index++;
			continue;
		}

		if (isdigit(input[_Index]))
		{
			CheckForNumber();
			continue;
		}

		if (isalpha(input[_Index]) || input[_Index] == '_')
		{
			CheckForIdentifier();
			continue;
		}


		if (!CheckForSymbol())
		{
			std::string error = std::format("Invalid token '{}' found at index {}", input[_Index], _Index);

			if (_Tokens.size() > 0)
			{
				error += std::format(" after token '{}'", _Tokens.back().Value);
			}

			return { _Tokens, error};
		}
	}

	return { _Tokens, std::string()};
}

