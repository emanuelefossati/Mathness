#pragma once

#include "./Results/List/List.h"
#include "./Results/Matrix/Matrix.h"
#include "./Utils/TypeDefinitions.h"
#include <variant>
#include <tuple>

using Result = std::variant<scalar_t, Matrix, List, error_t>;

constexpr bool IsScalar(const Result& result)
{
	return std::holds_alternative<scalar_t>(result);
}

constexpr bool IsMatrix(const Result& result)
{
	return std::holds_alternative<Matrix>(result);
}

constexpr bool IsList(const Result& result)
{
	return std::holds_alternative<List>(result);
}

constexpr bool IsError(const Result& result)
{
	return std::holds_alternative<error_t>(result);
}

static bool IsScalarInteger(const scalar_t & scalar)
{
	return scalar == std::floor(scalar);
}

constexpr scalar_t ResultToScalar(const Result& result)
{
	assert(IsScalar(result));
	return std::get<scalar_t>(result);
}

constexpr Matrix ResultToMatrix(const Result& result)
{
	assert(IsMatrix(result));
	return std::get<Matrix>(result);
}

constexpr List ResultToList(const Result& result)
{
	assert(IsList(result));
	return std::get<List>(result);
}


constexpr std::tuple<scalar_t, Matrix> RetrieveScalarAndMatrix(const Result& leftResult, const Result& rightResult)
{
	assert(IsScalar(leftResult) != IsScalar(rightResult));
	assert(IsMatrix(leftResult) != IsMatrix(rightResult));

	if (IsScalar(leftResult))
	{
		scalar_t leftScalar = ResultToScalar(leftResult);
		Matrix rightMatrix = ResultToMatrix(rightResult);

		return { leftScalar, rightMatrix };
	}
	else
	{
		Matrix leftMatrix = ResultToMatrix(leftResult);
		scalar_t rightScalar = ResultToScalar(rightResult);

		return { rightScalar, leftMatrix };
	}
}