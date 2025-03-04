#pragma once

#include <vector>
#include <assert.h>
#include "./Utils/TypeDefinitions.h"

struct Matrix
{
	std::vector<scalar_t> Elements;

	size_t Rows = 0;
	size_t Columns = 0;

	const scalar_t operator()(size_t i, size_t j) const;
	const Matrix operator+(const Matrix& other) const;
	const Matrix operator-(const Matrix& other) const;
	const Matrix operator*(scalar_t scalar) const;
	const Matrix operator*(const Matrix& other) const;
	const Matrix operator/(scalar_t scalar) const;
	const Matrix operator/(const Matrix& other) const;
	const Matrix operator^(scalar_t exponent) const;

	const Matrix Transpose() const;
	const Matrix Inverse() const;
	const scalar_t Determinant() const;
	const Matrix GetSubMatrix(size_t row, size_t column) const;
	
	const scalar_t Dot(const Matrix& other) const;
	const Matrix Cross(const Matrix& other) const;
	const scalar_t Norm() const;
	const Matrix Normalize() const;


	constexpr bool IsVector() const;
	constexpr bool IsSquare() const;
	constexpr bool IsIdentity() const;
	constexpr bool IsDiagonal() const;
	bool IsSingular() const;

	const std::string ToString() const;
	static const Matrix Identity(size_t size);

	
};