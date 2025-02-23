#pragma once
#include "./Utils/TokenDefinitions.h"

#include "./Absolute/AbsoluteNode.h"
#include "./Arccos/ArccosNode.h"
#include "./Arcsin/ArcsinNode.h"
#include "./Arctan/ArctanNode.h"
#include "./Cos/CosNode.h"
#include "./Exponential/ExpNode.h"
#include "./Factorial/FactorialNode.h"
#include "./Ln/LnNode.h"
#include "./Sin/SinNode.h"
#include "./Sqrt/SqrtNode.h"
#include "./Tan/TanNode.h"

std::shared_ptr<IUnaryNode> CreateUnaryFunction(TokenType tokenType)
{
	assert(IsTokenUnaryFunctionName(tokenType));

	switch (tokenType)
	{
	case TokenType::SQRT:
		return std::make_shared<SqrtNode>();

	case TokenType::ABS:
		return std::make_shared<AbsoluteNode>();

	case TokenType::EXP:
		return std::make_shared<ExpNode>();

	case TokenType::LN:
		return std::make_shared<LnNode>();

	case TokenType::FACTORIAL:
		return std::make_shared<FactorialNode>();

	case TokenType::SIN:
		return std::make_shared<SinNode>();

	case TokenType::COS:
		return std::make_shared<CosNode>();

	case TokenType::TAN:
		return std::make_shared<TanNode>();

	case TokenType::ASIN:
		return std::make_shared<ArcsinNode>();
	
	case TokenType::ACOS:
		return std::make_shared<ArccosNode>();

	case TokenType::ATAN:
		return std::make_shared<ArctanNode>();
	}
}