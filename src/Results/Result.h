#pragma once

#include "./Results/List/List.h"
#include "./Results/Matrix/Matrix.h"
#include "./Utils/TypeDefinitions.h"
#include <variant>
#include <tuple>

struct Result : public std::variant<scalar_t, Matrix, List, Error>
{
	using std::variant<scalar_t, Matrix, List, Error>::variant;

	constexpr bool IsScalar() const
	{
		return std::holds_alternative<scalar_t>(*this);
	}

	constexpr bool IsMatrix() const
	{
		return std::holds_alternative<Matrix>(*this);
	}

	constexpr bool IsList() const
	{
		return std::holds_alternative<List>(*this);
	}

	constexpr bool IsError() const
	{
		return std::holds_alternative<Error>(*this);
	}

	bool IsScalarInteger() const
	{
		assert(IsScalar());

		scalar_t value = ToScalar();

		return value == std::floor(value);
	}
	static bool IsScalarInteger(const Result& result)
	{
		assert(result.IsScalar());

		scalar_t value = result.ToScalar();

		return value == std::floor(value);
	}

	constexpr scalar_t ToScalar() const
	{
		assert(IsScalar());
		return std::get<scalar_t>(*this);
	}

	constexpr Matrix ToMatrix() const
	{
		assert(IsMatrix());
		return std::get<Matrix>(*this);
	}

	constexpr List ToList() const
	{
		assert(IsList());
		return std::get<List>(*this);
	}


	static constexpr std::tuple<scalar_t, Matrix> RetrieveScalarAndMatrix(const Result& leftResult, const Result& rightResult)
	{
		assert(leftResult.IsScalar() != rightResult.IsScalar());
		assert(leftResult.IsMatrix() != rightResult.IsMatrix());

		if (leftResult.IsScalar())
		{
			scalar_t leftScalar = leftResult.ToScalar();
			Matrix rightMatrix = rightResult.ToMatrix();

			return { leftScalar, rightMatrix };
		}
		else
		{
			Matrix leftMatrix = leftResult.ToMatrix();
			scalar_t rightScalar = rightResult.ToScalar();

			return { rightScalar, leftMatrix };
		}
	}
};



