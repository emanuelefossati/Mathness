#pragma once

#include <variant>
#include <memory>
#include <cassert>

#include "./AST/Node.h"


struct NodeResult : public std::variant<std::shared_ptr<INode>, Error>
{
	using std::variant<std::shared_ptr<INode>, Error>::variant;

	constexpr bool IsNode() const
	{
		return std::holds_alternative<std::shared_ptr<INode>>(*this);
	}

	constexpr bool IsError() const
	{
		return std::holds_alternative<Error>(*this);
	}

	std::shared_ptr<INode> ToNode() const
	{
		assert(IsNode());
		return std::get<std::shared_ptr<INode>>(*this);
	}

	constexpr Error ToError() const
	{
		assert(IsError());
		return std::get<Error>(*this);
	}
};

struct ParsingCheckResult : public std::variant<bool, Error>
{
	using std::variant<bool, Error>::variant;

	constexpr bool IsBool() const
	{
		return std::holds_alternative<bool>(*this);
	}

	constexpr bool IsError() const
	{
		return std::holds_alternative<Error>(*this);
	}

	constexpr bool ToBool() const
	{
		assert(IsBool());
		return std::get<bool>(*this);
	}

	constexpr Error ToError() const
	{
		assert(IsError());
		return std::get<Error>(*this);
	}
};