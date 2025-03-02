#pragma once

#include "./Utils/TokenDefinitions.h"

#include "Cross/CrossNode.h"
#include "Division/DivisionNode.h"
#include "Dot/DotNode.h"
#include "Log/LogNode.h"
#include "Modulus/ModulusNode.h"
#include "Power/PowerNode.h"
#include "Product/ProductNode.h"
#include "Sum/SumNode.h"
#include "Subtraction/SubtractionNode.h"

static std::shared_ptr<IBinaryNode> CreateBinaryNode(TokenType tokenType)
{
	assert(IsTokenBinaryFunctionName(tokenType) || IsTokenArithmeticOperator(tokenType));

	switch (tokenType)
	{
	case TokenType::VECTOR_PRODUCT:
		return std::make_shared<CrossNode>();

	case TokenType::DIVIDE:
		return std::make_shared<DivisionNode>();

	case TokenType::SCALAR_PRODUCT:
		return std::make_shared<DotNode>();

	case TokenType::LOG:
		return std::make_shared<LogNode>();

	case TokenType::MODULUS:
		return std::make_shared<ModulusNode>();

	case TokenType::POWER:
		return std::make_shared<PowerNode>();

	case TokenType::PRODUCT:
		return std::make_shared<ProductNode>();

	case TokenType::PLUS:
		return std::make_shared<SumNode>();

	case TokenType::MINUS:
		return std::make_shared<SubtractionNode>();
	}

}