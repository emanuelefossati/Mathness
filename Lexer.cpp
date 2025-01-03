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

	_Index = 0;
	_Tokens.clear();
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

static std::string ToLower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	return s;

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

	std::string identifier = _Input.substr(beginIndex, (size_t)_Index - beginIndex);
	TokenType type = GetIdentifierMap().find(ToLower(identifier)) != GetIdentifierMap().end() ? GetIdentifierMap().at(ToLower(identifier)) : TokenType::STORAGE;

	_Tokens.emplace_back(type, identifier);
}

std::tuple<std::vector<LexingToken>, std::optional<error_t>> Lexer::Lex(std::string& input)
{
	assert(input.size() > 0);

	Init(input);

	while (_Index < _Input.size())
	{
		if (isspace(_Input[_Index]))
		{
			_Index++;
			continue;
		}

		if (isdigit(_Input[_Index]))
		{
			CheckForNumber();
			continue;
		}

		if (isalpha(_Input[_Index]) || _Input[_Index] == '_')
		{
			CheckForIdentifier();

			continue;
		}


		if (!CheckForSymbol())
		{
			error_t error = std::format("Invalid token '{}' found at index {}", _Input[_Index], _Index);

			if (_Tokens.size() > 0)
			{
				error += std::format(" after token '{}'", _Tokens.back().Value);
			}

			return { _Tokens, error};
		}
	}

	return { _Tokens, std::nullopt};
}

