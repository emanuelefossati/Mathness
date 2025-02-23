#pragma once

#include <string>
#include <optional>
#include <tuple>
#include <array>
#include <vector>
#include <algorithm>
#include <string_view>
#include <unordered_map>
#include <format>
#include <numbers>

#include "./Utils//TypeDefinitions.h"



enum class TokenType
{
	NUMBER,

	//SYMBOLS


	PLUS,
	MINUS,
	PRODUCT,
	DIVIDE,
	MODULUS,
	POWER,

	OPEN_ROUND_BRACKET,
	CLOSE_ROUND_BRACKET,
	OPEN_SQUARE_BRACKET,
	CLOSE_SQUARE_BRACKET,
	OPEN_CURLY_BRACKET,
	CLOSE_CURLY_BRACKET,

	COMMA,
	COLON,
	SEMICOLON,

	EQUALS,

	//IDENTIFIERS


	STORAGE,

	PI,
	E,

	SCALAR_PRODUCT,
	VECTOR_PRODUCT,
	LOG,

	SQRT,
	ABS,
	NEGATE,
	EXP,
	LN,
	FACTORIAL,

	SIN,
	COS,
	TAN,
	ASIN,
	ACOS,
	ATAN,

	HELP,
	PRINT,
	EXIT,
};



static const std::unordered_map<char, TokenType>& GetSymbolMap()
{
	static const std::unordered_map<char, TokenType> single_character_symbol_map = {
		{ '+', TokenType::PLUS },
		{ '-', TokenType::MINUS },
		{ '*', TokenType::PRODUCT },
		{ '/', TokenType::DIVIDE },
		{ '%', TokenType::MODULUS },
		{ '^', TokenType::POWER },
		{ '(', TokenType::OPEN_ROUND_BRACKET },
		{ ')', TokenType::CLOSE_ROUND_BRACKET },
		{ '[', TokenType::OPEN_SQUARE_BRACKET },
		{ ']', TokenType::CLOSE_SQUARE_BRACKET },
		{ '{', TokenType::OPEN_CURLY_BRACKET },
		{ '}', TokenType::CLOSE_CURLY_BRACKET },
		{ ',', TokenType::COMMA },
		{ ':', TokenType::COLON },
		{ ';', TokenType::SEMICOLON },
		{ '=', TokenType::EQUALS },
	};

	return single_character_symbol_map;

}

static const std::unordered_map<std::string, TokenType>& GetIdentifierMap()
{
	static const std::unordered_map<std::string, TokenType> identifier_map = 
	{
		{ "pi", TokenType::PI },
		{ "e", TokenType::E },
		{ "dot", TokenType::SCALAR_PRODUCT },
		{ "cross", TokenType::VECTOR_PRODUCT },
		{ "log", TokenType::LOG },
		{ "sqrt", TokenType::SQRT },
		{ "abs", TokenType::ABS },
		{ "neg", TokenType::NEGATE },
		{ "exp", TokenType::EXP },
		{ "ln", TokenType::LN },
		{ "fact", TokenType::FACTORIAL },
		{ "sin", TokenType::SIN },
		{ "cos", TokenType::COS },
		{ "tan", TokenType::TAN },
		{ "asin", TokenType::ASIN },
		{ "acos", TokenType::ACOS },
		{ "atan", TokenType::ATAN },
		{ "help", TokenType::HELP },
		{ "print", TokenType::PRINT },
		{ "exit", TokenType::EXIT },
	};

	return identifier_map;
}



static bool IsTokenUnaryFunctionName(TokenType token)
{
	return
		token == TokenType::SQRT ||
		token == TokenType::ABS ||
		token == TokenType::NEGATE ||
		token == TokenType::EXP ||
		token == TokenType::LN ||
		token == TokenType::FACTORIAL ||
		token == TokenType::SIN ||
		token == TokenType::COS ||
		token == TokenType::TAN ||
		token == TokenType::ASIN ||
		token == TokenType::ACOS ||
		token == TokenType::ATAN;
}

static bool IsTokenBinaryFunctionName(TokenType token)
{
	return
		token == TokenType::SCALAR_PRODUCT ||
		token == TokenType::VECTOR_PRODUCT ||
		token == TokenType::LOG;
}

static bool IsTokenElementaryFunctionName(TokenType token)
{
	return
		IsTokenBinaryFunctionName(token) ||
		IsTokenUnaryFunctionName(token);
}

static bool IsTokenConstant(TokenType token)
{
	return
		token == TokenType::PI ||
		token == TokenType::E;
}

static bool IsTokenScalarValue(TokenType token)
{
	return
		token == TokenType::NUMBER ||
		IsTokenConstant(token);
}

static bool IsTokenValue(TokenType token)
{
	return
		IsTokenScalarValue(token) ||
		token == TokenType::STORAGE;
}

static bool IsTokenArithmeticOperator(TokenType token)
{
	return
		token == TokenType::PLUS ||
		token == TokenType::MINUS ||
		token == TokenType::PRODUCT ||
		token == TokenType::DIVIDE ||
		token == TokenType::MODULUS ||
		token == TokenType::POWER;
}



static bool IsTokenOpenBracket(TokenType token)
{
	return
		token == TokenType::OPEN_ROUND_BRACKET ||
		token == TokenType::OPEN_SQUARE_BRACKET ||
		token == TokenType::OPEN_CURLY_BRACKET;
}

static bool IsTokenMinus(TokenType token)
{
	return token == TokenType::MINUS;
}

static bool IsTokenExpressionEnd(TokenType token)
{
	return 
		token == TokenType::SEMICOLON ||
		token == TokenType::COMMA ||
		token == TokenType::CLOSE_ROUND_BRACKET ||
		token == TokenType::CLOSE_SQUARE_BRACKET ||
		token == TokenType::CLOSE_CURLY_BRACKET;
}

struct LexingToken
{
	TokenType Type;
	std::string Value;

	std::string ToString() const
	{

		std::string s_type;
		switch (Type)
		{
		case TokenType::NUMBER:
			s_type = "NUMBER";
			break;

		case TokenType::PLUS:
			s_type = "PLUS";
			break;

		case TokenType::MINUS:
			s_type = "MINUS";
			break;

		case TokenType::PRODUCT:
			s_type = "PRODUCT";
			break;

		case TokenType::DIVIDE:
			s_type = "DIVIDE";
			break;

		case TokenType::MODULUS:
			s_type = "MODULUS";
			break;

		case TokenType::POWER:
			s_type = "POWER";
			break;


		case TokenType::OPEN_ROUND_BRACKET:
			s_type = "OPEN_ROUND_BRACKET";
			break;

		case TokenType::CLOSE_ROUND_BRACKET:
			s_type = "CLOSE_ROUND_BRACKET";
			break;

		case TokenType::OPEN_SQUARE_BRACKET:
			s_type = "OPEN_SQUARE_BRACKET";
			break;

		case TokenType::CLOSE_SQUARE_BRACKET:
			s_type = "CLOSE_SQUARE_BRACKET";
			break;

		case TokenType::OPEN_CURLY_BRACKET:
			s_type = "OPEN_CURLY_BRACKET";
			break;

		case TokenType::CLOSE_CURLY_BRACKET:
			s_type = "CLOSE_CURLY_BRACKET";
			break;

		case TokenType::COMMA:
			s_type = "COMMA";
			break;

		case TokenType::COLON:
			s_type = "COLON";
			break;

		case TokenType::SEMICOLON:
			s_type = "SEMICOLON";
			break;

		case TokenType::EQUALS:
			s_type = "EQUALS";
			break;

		case TokenType::STORAGE:
			s_type = "STORAGE";
			break;

		case TokenType::PI:
			s_type = "PI";
			break;

		case TokenType::E:
			s_type = "E";
			break;

		case TokenType::SCALAR_PRODUCT:
			s_type = "SCALAR_PRODUCT";
			break;

		case TokenType::VECTOR_PRODUCT:
			s_type = "VECTOR_PRODUCT";
			break;

		case TokenType::LOG:
			s_type = "LOG";
			break;

		case TokenType::SQRT:
			s_type = "SQRT";
			break;

		case TokenType::ABS:
			s_type = "ABS";
			break;

		case TokenType::NEGATE:
			s_type = "NEGATE";
			break;

		case TokenType::EXP:
			s_type = "EXP";
			break;

		case TokenType::LN:
			s_type = "LN";
			break;

		case TokenType::FACTORIAL:
			s_type = "FACTORIAL";
			break;

		case TokenType::SIN:
			s_type = "SIN";
			break;

		case TokenType::COS:
			s_type = "COS";
			break;

		case TokenType::TAN:
			s_type = "TAN";
			break;

		case TokenType::ASIN:
			s_type = "ASIN";
			break;

		case TokenType::ACOS:
			s_type = "ACOS";
			break;

		case TokenType::ATAN:
			s_type = "ATAN";
			break;

		case TokenType::HELP:
			s_type = "HELP";
			break;

		case TokenType::PRINT:
			s_type = "PRINT";
			break;

		case TokenType::EXIT:
			s_type = "EXIT";
			break;

		default:
			s_type = "UNKNOWN";
			break;
		}

		return std::format("[{}]:[{}]", Value, s_type);

	}
};

static scalar_t GetScalarValue(LexingToken token)
{
	if (token.Type == TokenType::NUMBER)
	{
		return std::stof(token.Value);
	}

	switch (token.Type)
	{
	case TokenType::PI:
		return std::numbers::pi;
	case TokenType::E:
		return std::numbers::e;
	default:
		return 0;
	}
}



