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



enum class TokenType
{
	IDENTIFIER,
	NUMBER,

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
};

enum class IdentifierType
{
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

//std::unordered_map<std::string_view, TokenType> constant_map = {
//	{ "pi", TokenType::PI },
//	{ "e", TokenType::E },
//};
//

//std::unordered_map<std::string_view, TokenType> binary_operator_symbol_map = {
//	{ "+", TokenType::PLUS },
//	{ "-", TokenType::MINUS },
//	{ "*", TokenType::PRODUCT },
//	{ "/", TokenType::DIVIDE },
//	{ "%", TokenType::MODULUS },
//	{ "^", TokenType::POWER },
//};
//std::unordered_map<std::string_view, TokenType> binary_operator_keyword_map = {
//	{ "dot", TokenType::SCALAR_PRODUCT },
//	{ "cross", TokenType::VECTOR_PRODUCT },
//	{ "log", TokenType::LOG },
//};
//
//std::unordered_map<std::string_view, TokenType> unary_operator_keyword_map = {
//	{ "sqrt", TokenType::SQRT },
//	{ "abs", TokenType::ABS },
//	{ "neg", TokenType::NEGATE },
//	{ "exp", TokenType::EXP },
//	{ "ln", TokenType::LN },
//	{ "fact", TokenType::FACTORIAL },
//};
//
//std::unordered_map<std::string_view, TokenType> trigonometric_function_map = {
//	{ "sin", TokenType::SIN },
//	{ "cos", TokenType::COS },
//	{ "tan", TokenType::TAN },
//	{ "asin", TokenType::ASIN },
//	{ "acos", TokenType::ACOS },
//	{ "atan", TokenType::ATAN },
//};
//
//std::unordered_map<std::string_view, TokenType> bracket_map = 
//{
//	{ "(", TokenType::OPEN_ROUND_BRACKET },
//	{ ")", TokenType::CLOSE_ROUND_BRACKET },
//	{ "[", TokenType::OPEN_SQUARE_BRACKET },
//	{ "]", TokenType::CLOSE_SQUARE_BRACKET },
//	{ "{", TokenType::OPEN_CURLY_BRACKET },
//	{ "}", TokenType::CLOSE_CURLY_BRACKET },
//};
//
//std::unordered_map<std::string_view, TokenType> extra_symbol_map = {
//	{ ",", TokenType::COMMA },
//	{ "=", TokenType::EQUALS },
//};
//
//std::unordered_map<std::string_view, TokenType> command_map = {
//	{ "help", TokenType::HELP },
//	{ "print", TokenType::PRINT },
//	{ "exit", TokenType::EXIT },
//};



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

		case TokenType::IDENTIFIER:
			s_type = "IDENTIFIER";
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

		default:
			s_type = "UNKNOWN";
			break;
		}

		return std::format("[{}]:[{}]", Value, s_type);

	}
};

